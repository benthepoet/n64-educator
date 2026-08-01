# L16 — Vertex colors in Tiny3D

::: tip Goal
Build a small **heightfield grid** where each vertex has its own color. Toggle paint OFF to see how much variation came from vertex RGBA alone — the N64 terrain mindset on real hardware.
:::

## In plain English (L10 on the console)

L10 showed two triangles: flat vs vertex-tinted.  
L16 scales that to a **grid of quads** with soft hills:

- Same normals (up)  
- Same lights  
- Different **RGBA per vertex** → grass / dirt style blotches  

Press **A** to replace all colors with a flat greenish white. The *shape* stays; the *paint* vanishes. That’s the lesson.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l16-vertex-color
```

| Input | Effect |
|-------|--------|
| Stick | Orbit camera |
| **A** | Toggle vertex paint ON/OFF |

---

## Why this matters for Starshard Cove

Island ground:

1. Optional small tiling texture (Module 3)  
2. **Vertex colors** for paths, moisture, fake AO  
3. Multiply in the material/combiner  

You’re practicing step 2 in pure code first so Blender vertex paint later isn’t abstract.

---

## Implementation sketch

For each grid cell, four corners get:

- `pos` from grid + height function  
- `rgba` from a paint function  
- `normal` ≈ up  

Then two triangles per cell — same as L09, many times.

::: tip Performance honesty
Re-loading many small batches per frame is fine for a teaching grid, not for a final open world. Real levels use fewer, optimized meshes (importer + strips). Learn the idea first.
:::

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “I need a texture to see color” | Vertex color alone is enough (L16 proves it) |
| “Paint is lighting” | Lighting still applies; paint multiplies with it |
| “Blender is required for vertex color” | No — but artists *prefer* painting there (Module 3) |

---

## Exercises

1. Toggle A until you can teach the difference to someone else.  
2. Bias `paint_noise` toward brown — “paths.”  
3. Module 1 checkpoint + Module 2: explain TMEM + vertex paint in two sentences.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l16-vertex-color/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l16-vertex-color/Makefile) · [`lessons/l16-vertex-color/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l16-vertex-color/src/main.c)

::: details `lessons/l16-vertex-color/Makefile`
<<< ../../../lessons/l16-vertex-color/Makefile{make}
:::

::: details `lessons/l16-vertex-color/src/main.c`
<<< ../../../lessons/l16-vertex-color/src/main.c{c}
:::

## What you learned

- Vertex color on Tiny3D geometry  
- Terrain variation without unique mega-textures  
- Orbit + paint debugging  

## Module 2 checkpoint (self-test)

1. Why attach **both** color and depth buffers for 3D?  
2. What does `look_at(eye, target, up)` build?  
3. Ambient vs directional — one sentence each.  
4. Why convert GLB → `.t3dm` at build time?  
5. Why vertex-paint a floor instead of one huge texture?

If those feel answerable, Module 3 (Blender + Fast64) is next — making art the pipeline expects.

## Next

[Module 3 — Blender for N64](/guide/m3/overview): survival kit, vertex paint, Fast64/export, rig, idle/walk, asset lab.
