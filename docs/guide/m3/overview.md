# Module 3 — Blender for N64

::: tip Goal
Go from **zero Blender** to exporting meshes that Tiny3D can draw: modeling, UVs, **vertex paint**, Fast64 materials (or course inject helper), simple **rig + idle/walk**, and a live asset lab on console.
:::

## If art is scary

This module is written for programmers who have never opened Blender. You will:

1. Survive the UI (orbit, select, move)  
2. Make **ugly-but-valid** low-poly props  
3. Export through a **fixed checklist**  
4. See results in the **M3 Asset Lab** ROM  

Course ships **placeholder props** so you can finish even if your first exports fail.

## Prerequisites

- Module 2 complete (you can load `rom:/*.t3dm`)  
- Blender **4.2+** (course tested on 5.x batch scripts too)  
- Fast64 **recommended** for real materials; without it use `scripts/gltf_inject_f3d.py` (course helper)  

```bash
blender --version
source scripts/env.sh
ls "$N64_INST/bin/gltf_to_t3d"
```

## Lessons

| Lesson | Focus |
|--------|--------|
| [L17 — Blender survival](./l17-survival) | Navigate, modes, scale convention |
| [L18 — Model starshard](./l18-starshard) | First prop from primitives |
| [L19 — UVs & textures](./l19-uvs) | Unwrap + small power-of-two images |
| [L20 — Vertex paint](./l20-vertex-paint) | `Col` layer, paths/tints |
| [L21 — Fast64 & export](./l21-fast64-export) | Materials, custom props, glTF → t3dm |
| [L22 — Player & platform](./l22-player-platform) | Blockout character + modular piece |
| [L23 — Level layout](./l23-level) | Assemble island + paint pass |
| [L24 — Rigging](./l24-rigging) | Simple armature, weights, Tiny3D limits |
| [L25 — Idle & walk](./l25-animation) | Looping actions + export |
| [Checkpoint — Asset lab](./checkpoint) | ROM gallery of your (or course) art |

## Course pipeline (cheat sheet)

```text
Blender .blend
   → glTF Binary (.glb)  [Include Custom Properties if Fast64]
   → (optional) scripts/gltf_inject_f3d.py   # no Fast64 fallback
   → gltf_to_t3d → model.t3dm (+ sprites)
   → pack into ROM DFS → t3d_model_load("rom:/…")
```

Regenerate placeholders anytime:

```bash
./scripts/build-course-assets.sh
```

## Conventions reminder

| Rule | Value |
|------|--------|
| World up | +Z in Blender often maps differently — apply transforms; course props use Blender Z-up export that Tiny3D samples handle |
| Texture sizes | 32×32 or 64×64 first |
| Vertex color layer | Name **`Col`** (and **`Alpha`** if needed) |
| Poly budget | Think dozens–low hundreds of tris per prop at first |

Exact axis mapping can surprise you once — always **view in the asset lab** after export.

## What Module 3 is *not*

- Not a full character-animation degree  
- Not PBR / Cycles rendering for film  
- Not “make it look like Zelda day one”

Ugly and readable beats pretty and broken.

Start: [L17 — Blender survival](./l17-survival).
