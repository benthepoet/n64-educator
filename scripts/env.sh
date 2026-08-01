# Source this file to use the n64-educator toolchain layout.
#
#   source scripts/env.sh
#
# Default install root: $HOME/n64-dev (user-local, no root required).
# Override with N64_DEV if you put the toolchain elsewhere.

: "${N64_DEV:=$HOME/n64-dev}"
export N64_INST="${N64_INST:-$N64_DEV/toolchain}"
export T3D_INST="${T3D_INST:-$N64_DEV/tiny3d}"
export PATH="$N64_INST/bin${PATH:+:$PATH}"

if [[ ! -f "$N64_INST/include/n64.mk" ]]; then
  echo "warning: n64.mk not found at $N64_INST/include/n64.mk" >&2
  echo "         Run: ./scripts/install-toolchain.sh" >&2
fi
