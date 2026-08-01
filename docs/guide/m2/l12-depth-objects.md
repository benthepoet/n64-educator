# L12 — Depth buffer + multiple objects

::: tip Goal
Treat each object as **mesh + matrix**, draw more than one, and *see* what the **depth buffer** does when you toggle it.
:::

## In plain English

**Painter’s algorithm** = draw far things first, near things last (like watercolors). It’s fragile when objects cross.

**Z-buffer (depth buffer)** = for every pixel, remember the closest surface so far. New pixels only draw if they’re closer. Draw order becomes much less stressful.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l12-depth-objects
```

| Object | Role |
|--------|------|
| **Cyan** | Farther, bigger |
| **Red** | Nearer, slides left/right |

| Input | Effect |
|-------|--------|
| **A** | Toggle depth test ON/OFF |

::: tip Try this
With depth **ON**, red correctly stays in front even though we sometimes draw cyan first.  
With depth **OFF**, whichever draws **last** paints on top — red/cyan “pop” wrong.
:::

---

## Entity mindset

```text
Actor / entity ≈ {
  geometry (verts or model),
  transform (matrix / SRT),
  maybe materials later
}
```

Starshard Cove will be a list of entities (player, shards, island), each with a matrix — same idea as two quads here.

---

## API notes

- `T3D_FLAG_DEPTH` — enable depth testing/writing for draws.  
- Always **clear depth** each frame (`t3d_screen_clear_depth`) or you’ll see ghosts.  
- `t3d_matrix_set(mat, true)` — set current model matrix for following verts.  
- `rdpq_mode_combiner(RDPQ_COMBINER_SHADE)` — required for hand-built colored tris (same as L11).

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Depth is optional decoration” | It’s how 3D stays coherent |
| “I must sort all triangles” | Z-buffer handles most cases; transparency is the hard leftover |
| “Z-fighting flicker” | Two surfaces same depth — separate them or bias later |

---

## Exercises

1. Toggle A until the difference is obvious; explain it out loud.  
2. Swap draw order of red/cyan with depth ON — should still look correct.  
3. Add a third quad (copy-paste pattern) at another Z.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l12-depth-objects/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l12-depth-objects/Makefile) · [`lessons/l12-depth-objects/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l12-depth-objects/src/main.c)

::: details `lessons/l12-depth-objects/Makefile`
<<< ../../../lessons/l12-depth-objects/Makefile{make}
:::

::: details `lessons/l12-depth-objects/src/main.c`
<<< ../../../lessons/l12-depth-objects/src/main.c{c}
:::

## What you learned

- Multiple model matrices  
- Depth buffer purpose  
- Clear color **and** depth every frame  

## Next

[L13 — Orbit camera](./l13-orbit-camera) — you fly the view around a tiny scene.
