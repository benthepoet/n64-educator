# L15 — Loading a model

::: tip Goal
Run the **asset pipeline**: `glb` → `.t3dm` on the ROM → `t3d_model_load` → `t3d_model_draw`. This is how real props enter the game.
:::

## In plain English

Hand-packing `T3DVertPacked` is great for learning. Games load **files**.

Tiny3D’s path:

```text
Blender (or any tool) → glTF/GLB
        ↓  gltf_to_t3d  (build time)
     model.t3dm + textures
        ↓  packed into DFS
   rom:/model.t3dm
        ↓  t3d_model_load
     T3DModel* in RAM
        ↓  t3d_model_draw
     triangles on screen
```

This lesson ships a **known-good** sample model (from Tiny3D’s examples) so you aren’t blocked on Blender yet. Module 3 teaches making your own.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l15-load-model
```

A textured/lit mesh spinning. Stick X adjusts spin speed.

On build logs you should spot:

```text
[T3D-MODEL] filesystem/model.t3dm
[SPRITE]    ...
[DFS] ...
```

---

## Code shape

```c
dfs_init(DFS_DEFAULT_LOCATION);
T3DModel *model = t3d_model_load("rom:/model.t3dm");

rspq_block_begin();
t3d_model_draw(model);
rspq_block_t *dpl = rspq_block_end();

/* each frame after matrix set: */
rspq_block_run(dpl);
```

Materials and texture binds are largely **inside** the model format — less manual RDP than L04 sprites.

### Matrix buffering

We allocate **one matrix per framebuffer** and rotate the index each frame. That way the CPU doesn’t overwrite a matrix the RSP still needs. Remember this pattern when things “flicker” or “jitter” under high load.

---

## Makefile / assets

`assets/*.glb` auto-convert when `USE_T3D := 1` (see `common/lesson.mk`).  
PNGs in `assets/` become sprites the importer expects.

::: warning Don’t load host paths
Always `rom:/...` after DFS init — same rule as L04.
:::

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “GLB works at runtime” | Not raw — convert to `.t3dm` at **build** |
| “Where did textures go?” | Converted to sprites and referenced by the model |
| “Black model” | Lights too dark, scale wrong, or camera inside mesh |
| “Scale is insane” | Sample uses `modelScale = 0.1f` — authoring units vary |

---

## Exercises

1. Watch the build print `[T3D-MODEL]`.  
2. Change camera Z; reframe the subject.  
3. (Stretch) Copy a second glb later from your own export (Module 3).


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l15-load-model/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l15-load-model/Makefile) · [`lessons/l15-load-model/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l15-load-model/src/main.c)

::: details `lessons/l15-load-model/Makefile`
<<< ../../../lessons/l15-load-model/Makefile{make}
:::

::: details `lessons/l15-load-model/src/main.c`
<<< ../../../lessons/l15-load-model/src/main.c{c}
:::

## What you learned

- Offline model conversion  
- `t3d_model_load` / `t3d_model_draw`  
- DFS + matrix buffering habits  

## Next

[L16 — Vertex color](./l16-vertex-color) — terrain paint on the 3D pipeline (Starshard Cove ground energy).
