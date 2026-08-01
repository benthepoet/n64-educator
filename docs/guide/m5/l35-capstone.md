# L35 — Capstone assembly: Starshard Cove

::: tip Goal
Ship the full mini-game: **8 shards**, ranks, music, HUD, states — everything the course taught.
:::

## Build

```bash
source scripts/env.sh
make -C capstone/starshard-cove
# → capstone/starshard-cove/starshard_cove.z64
```

## Feature map

| Feature | Module roots |
|---------|----------------|
| Toolchain / ROM | M0 |
| Vectors / matrices / camera | M1 |
| Tiny3D draw / lights / models | M2 |
| Blender assets | M3 |
| Move, anim, follow cam, collide, state | M4 |
| Audio, HUD, juice, 8 shards, ranks | M5 |

## Controls

| Input | Action |
|-------|--------|
| START | Title→Play, pause, win→title |
| Stick | Move |
| C-left/right | Orbit camera |

## Ranks

| Rank | Time |
|------|------|
| Gold | &lt; 45 s |
| Silver | &lt; 90 s |
| Bronze | otherwise |

Tune freely.

## Art swap

Replace `assets/*.t3dm` (and audio) in `capstone/starshard-cove/assets/` with your exports from Module 3. Keep names or update `t3d_model_load` paths.

## Next

[L36 — Shipping](./l36-shipping).
