# Module 2 — Tiny3D first light

::: tip Goal
Draw **real 3D** on the N64 with **Tiny3D**: viewport (camera + lens), depth, lights, loaded models, and vertex colors — reusing Module 1 vocabulary.
:::

## If you’re nervous

Module 1 was the language. Module 2 is speaking it on the console.

You already know:

| Module 1 | Appears here as |
|----------|-----------------|
| Vectors / points | Camera eye, light direction, positions |
| Matrices T/R/S | `t3d_mat4fp_from_srt_euler` / model matrices |
| View + projection | `t3d_viewport_look_at` + `t3d_viewport_set_projection` |
| Vertices + color | `T3DVertPacked` and painted terrain |
| Ambient / modulate | `t3d_light_set_*` and vertex RGBA |

New pieces are mostly **plumbing**: init Tiny3D, attach color+depth buffers, uncached matrices for the RSP.

## Prerequisites

- Module 0 + Module 1  
- Toolchain with **Tiny3D** installed (`./scripts/install-toolchain.sh` or [Setup](/guide/setup))  
- `source scripts/env.sh` so `N64_INST` is set  

Confirm:

```bash
ls "$N64_INST/include/t3d.mk"
ls "$N64_INST/bin/gltf_to_t3d"
```

## Lessons

| Lesson | Idea | ROM |
|--------|------|-----|
| [L11 — First quad](./l11-t3d-quad) | Init Tiny3D, viewport, lit spinning quad | Auto spin |
| [L12 — Depth + objects](./l12-depth-objects) | Two entities, Z-buffer, draw order | A toggles depth |
| [L13 — Orbit camera](./l13-orbit-camera) | Interactive 3D camera | Stick / C-up/dn |
| [L14 — Lighting](./l14-lighting) | Ambient + directional | Stick + A |
| [L15 — Load model](./l15-load-model) | glTF → `.t3dm` → draw | Stick spin |
| [L16 — Vertex color](./l16-vertex-color) | Painted terrain grid | A toggles paint |

## How to study

1. Read the **NOOB MAP** at the top of each lesson page.  
2. Build and run the ROM.  
3. Only then dig into API details.  
4. Keep [L05 hardware](/guide/m0/l05-hardware) in mind: CPU records, RSP/RDP draw, VI shows.

## Build flags

Module 2 lessons set:

```make
USE_T3D := 1
```

in their Makefile so `common/lesson.mk` links Tiny3D (and does **not** force Tiny3D into Module 0/1 ROMs).

## Build all Module 2 ROMs

```bash
source scripts/env.sh
make l11 l12 l13 l14 l15 l16
```

Start: [L11 — First quad](./l11-t3d-quad).
