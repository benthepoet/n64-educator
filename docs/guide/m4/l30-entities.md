# L30 — Entities & spawn table

::: tip Goal
Stop hard-coding objects in random variables. Use a **spawn table** and an **entity array**.
:::

## In plain English

```c
typedef struct {
  EntType type;
  fm_vec3_t pos;
  bool alive;
  ...
} Entity;

const Spawn SPAWNS[] = { {ENT_SHARD, 3,0.7,1.5}, ... };
```

Gameplay and drawing both **iterate entities**. Later, Blender empties can fill the same table.

## What you will see

```bash
make -C lessons/l30-entities
```

Player, shards, and a platform all come from `SPAWNS[]`.

Draw still uses **one matrix push/pop per entity** (and `t3d_skeleton_use` for the
player). Scales match L26/L28 (island ~0.032, snake ~0.02, starshard ~0.02).

Camera: move relative to the **lagged eye** (what you see), not player yaw.
`camYaw` is C-orbit only. Soft bounds are a **box**, not a radial cylinder
(radial walls skate the rim and look like the camera is spinning). Snake model
uses **`-yaw`**. `FILTERS_RESAMPLE` + opaque clear.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l30-entities/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l30-entities/Makefile) · [`lessons/l30-entities/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l30-entities/src/main.c)

::: details `lessons/l30-entities/Makefile`
<<< ../../../lessons/l30-entities/Makefile{make}
:::

::: details `lessons/l30-entities/src/main.c`
<<< ../../../lessons/l30-entities/src/main.c{c}
:::

## Next

[L31 — Game state](./l31-game-state).
