#!/usr/bin/env bash
# Generate Module 3 teaching props and convert them for Tiny3D.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="$ROOT/assets-src/blender/generated"
STAGE="$ROOT/assets-src/export"
FS_DIR="$ROOT/lessons/m3-asset-lab/assets"

source "$ROOT/scripts/env.sh" 2>/dev/null || true
: "${N64_INST:=$HOME/n64-dev/toolchain}"
export PATH="$N64_INST/bin${PATH:+:$PATH}"

mkdir -p "$OUT" "$STAGE" "$FS_DIR"

echo "==> Blender generate"
blender -b -P "$ROOT/scripts/blender_make_course_props.py" -- --out-dir "$OUT"

echo "==> Inject f3d_mat + convert"
for name in starshard platform island player_static; do
  glb="$OUT/${name}.glb"
  inj="$STAGE/${name}.glb"
  t3dm="$FS_DIR/${name}.t3dm"
  python3 "$ROOT/scripts/gltf_inject_f3d.py" "$glb" "$inj"
  echo "    [T3D] $name"
  gltf_to_t3d "$inj" "$t3dm"
  # mkasset compress into place (gltf_to_t3d may write raw; lesson makefile also runs mkasset)
  if command -v mkasset >/dev/null; then
    mkasset -c 2 -o "$FS_DIR" "$t3dm" || true
  fi
done

# Reference animated character (CC0 via Tiny3D examples / Quaternius)
REF_SNAKE="$HOME/n64-dev/tiny3d/examples/08_animation/assets/snake.glb"
if [[ -f "$REF_SNAKE" ]]; then
  echo "==> Copy reference snake (idle/walk) for animation lessons"
  cp "$REF_SNAKE" "$FS_DIR/player_anim.glb"
  # Already Fast64-ready — convert directly
  gltf_to_t3d "$FS_DIR/player_anim.glb" "$FS_DIR/player_anim.t3dm"
  mkasset -c 2 -o "$FS_DIR" "$FS_DIR/player_anim.t3dm" || true
  # textures next to it
  cp "$HOME/n64-dev/tiny3d/examples/08_animation/assets/"*.png "$FS_DIR/" 2>/dev/null || true
fi

# Also copy map for lab backdrop optional
REF_MAP="$HOME/n64-dev/tiny3d/examples/08_animation/assets/map.glb"
if [[ -f "$REF_MAP" ]]; then
  cp "$REF_MAP" "$FS_DIR/map.glb"
  gltf_to_t3d "$FS_DIR/map.glb" "$FS_DIR/map.t3dm"
  mkasset -c 2 -o "$FS_DIR" "$FS_DIR/map.t3dm" || true
fi

echo "Done. Models in $FS_DIR"
ls -la "$FS_DIR"
