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

## Next

[L31 — Game state](./l31-game-state).
