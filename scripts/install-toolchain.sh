#!/usr/bin/env bash
# Install a user-local N64 toolchain (no root required) into $HOME/n64-dev:
#   - GCC MIPS toolchain (from libdragon continuous prerelease .deb, extracted)
#   - libdragon preview (built + installed into the toolchain prefix)
#   - Tiny3D (built + installed into the same prefix)
#
# Usage:
#   ./scripts/install-toolchain.sh
#   N64_DEV=/path/to/n64-dev ./scripts/install-toolchain.sh
set -euo pipefail

N64_DEV="${N64_DEV:-$HOME/n64-dev}"
TOOLCHAIN="$N64_DEV/toolchain"
DOWNLOADS="$N64_DEV/downloads"
JOBS="${JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)}"

ARCH="$(uname -m)"
case "$ARCH" in
  x86_64|amd64) DEB_ARCH=x86_64 ;;
  aarch64|arm64) DEB_ARCH=aarch64 ;;
  *)
    echo "error: unsupported host arch: $ARCH" >&2
    exit 1
    ;;
esac

DEB_NAME="gcc-toolchain-mips64-${DEB_ARCH}.deb"
DEB_URL="https://github.com/DragonMinded/libdragon/releases/download/toolchain-continuous-prerelease/${DEB_NAME}"

# Course-verified pins (update docs/reference/versions.md when changing)
LIBDRAGON_REF="${LIBDRAGON_REF:-preview}"
TINY3D_REF="${TINY3D_REF:-main}"

mkdir -p "$DOWNLOADS" "$TOOLCHAIN"

echo "==> Toolchain prefix: $TOOLCHAIN"

# --- GCC toolchain ---
if [[ ! -x "$TOOLCHAIN/bin/mips64-elf-gcc" ]]; then
  echo "==> Downloading $DEB_NAME"
  curl -L --fail -o "$DOWNLOADS/$DEB_NAME" "$DEB_URL"
  echo "==> Extracting toolchain (no root)"
  TMP=$(mktemp -d)
  (
    cd "$TMP"
    ar x "$DOWNLOADS/$DEB_NAME"
    DATA=$(ls data.tar.* | head -1)
    tar -xf "$DATA"
    if [[ -d opt/libdragon ]]; then
      # rsync optional; cp -a works
      cp -a opt/libdragon/. "$TOOLCHAIN/"
    else
      echo "error: unexpected deb layout" >&2
      find . -maxdepth 3 -type d | head -40 >&2
      exit 1
    fi
  )
  rm -rf "$TMP"
else
  echo "==> GCC already present at $TOOLCHAIN/bin/mips64-elf-gcc"
fi

export N64_INST="$TOOLCHAIN"
export PATH="$N64_INST/bin:$PATH"
mips64-elf-gcc --version | head -1

# --- libdragon preview ---
if [[ ! -d "$N64_DEV/libdragon/.git" ]]; then
  echo "==> Cloning libdragon ($LIBDRAGON_REF)"
  git clone --depth 1 --branch "$LIBDRAGON_REF" \
    https://github.com/DragonMinded/libdragon.git "$N64_DEV/libdragon"
else
  echo "==> Updating libdragon"
  git -C "$N64_DEV/libdragon" fetch origin "$LIBDRAGON_REF"
  git -C "$N64_DEV/libdragon" checkout "$LIBDRAGON_REF"
  git -C "$N64_DEV/libdragon" pull --ff-only || true
fi

echo "==> Building libdragon @ $(git -C "$N64_DEV/libdragon" rev-parse --short HEAD)"
make -C "$N64_DEV/libdragon" -j"$JOBS"
make -C "$N64_DEV/libdragon" install
make -C "$N64_DEV/libdragon/tools" -j"$JOBS"
make -C "$N64_DEV/libdragon/tools" install

# --- Tiny3D ---
if [[ ! -d "$N64_DEV/tiny3d/.git" ]]; then
  echo "==> Cloning Tiny3D ($TINY3D_REF)"
  git clone --depth 1 --branch "$TINY3D_REF" \
    https://github.com/HailToDodongo/tiny3d.git "$N64_DEV/tiny3d"
else
  echo "==> Updating Tiny3D"
  git -C "$N64_DEV/tiny3d" fetch origin "$TINY3D_REF"
  git -C "$N64_DEV/tiny3d" checkout "$TINY3D_REF"
  git -C "$N64_DEV/tiny3d" pull --ff-only || true
fi

echo "==> Building Tiny3D @ $(git -C "$N64_DEV/tiny3d" rev-parse --short HEAD)"
(
  cd "$N64_DEV/tiny3d"
  ./build.sh
)

echo
echo "Done."
echo "  N64_INST=$N64_INST"
echo "  libdragon: $(git -C "$N64_DEV/libdragon" rev-parse HEAD)"
echo "  Tiny3D:    $(git -C "$N64_DEV/tiny3d" rev-parse HEAD)"
echo
echo "Activate in this shell:"
echo "  source $(cd "$(dirname "$0")" && pwd)/env.sh"
echo "Then:"
echo "  make -C lessons/l01-hello-rom"
