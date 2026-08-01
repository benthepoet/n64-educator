# L14 — Lighting in Tiny3D

::: tip Goal
Drive **ambient** and **directional** lights live. See why **normals** matter (faces point somewhere).
:::

## In plain English (L10 → hardware)

| L10 word | Tiny3D call |
|----------|-------------|
| Ambient | `t3d_light_set_ambient(rgba)` |
| Directional “sun” | `t3d_light_set_directional(index, color, dir)` |
| How many extras | `t3d_light_set_count(n)` |

Directional light uses each vertex’s **normal**: faces toward the sun get brighter. That’s why we packed normals in L11–L13, not only colors.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l14-lighting
```

Two white faces (front + top) of a blocky shape, slowly spinning.

| Input | Effect |
|-------|--------|
| Stick Y | Ambient level |
| Stick X | Orbit the sun direction |
| **A** | Toggle directional light |

::: tip Try this
Turn directional **OFF** (A). Only ambient left — flat, no “shape from shading.”  
Turn it **ON** and move Stick X — the bright side crawls around the model.
:::

---

## Normals for humans

A **normal** is an arrow sticking out of the surface: “which way is outward?”

- Floor: normal ≈ (0, 1, 0)  
- Wall facing +Z: normal ≈ (0, 0, 1)  

If normals are wrong, lighting looks inside-out or dead. Blender “recalculate normals” (Module 3) fixes many export issues.

---

## Color multiply chain (again)

Roughly:

```text
out ≈ vertex_color * (ambient + directional * n·L)
```

White vertices show lighting clearly. Dark vertex paint (L16) multiplies on top — N64 terrain trick.

---

## Common noob confusions

Draw pattern matches L12/L13: **`matrix_push` → `vert_load` → `matrix_pop` → tris**, and
`RDPQ_COMBINER_SHADE`.

| Feeling | Reality |
|---------|---------|
| “Light position” | Directional lights have **direction**, not a point in space (point lights come later) |
| “Ambient 0 is realistic” | Often too harsh for N64 readability; keep a little fill |
| “Normals optional” | Without them, directional lighting can’t work |
| “Mesh is pure black” | Check `RDPQ_COMBINER_SHADE` and ambient; shade × light can crush color |
| “No mesh at all” | Check push/pop vs `matrix_set` (camera stack) |

---

## Exercises

1. Ambient 0 + dir ON — dramatic. Ambient high + dir OFF — chalky.  
2. Change `colorDir` to pure blue — moonlit vibe.  
3. Predict: top face vs front face, which is brighter for light from above?


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l14-lighting/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l14-lighting/Makefile) · [`lessons/l14-lighting/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l14-lighting/src/main.c)

::: details `lessons/l14-lighting/Makefile`
<<< ../../../lessons/l14-lighting/Makefile{make}
:::

::: details `lessons/l14-lighting/src/main.c`
<<< ../../../lessons/l14-lighting/src/main.c{c}
:::

## What you learned

- Ambient vs directional in Tiny3D  
- Normals feed directional lighting  
- Interactive light debugging with the stick  

## Next

[L15 — Load model](./l15-load-model) — stop hand-authoring every vertex; load art from Blender/glTF.
