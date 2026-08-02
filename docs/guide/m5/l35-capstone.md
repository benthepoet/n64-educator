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
| Stick | Move (relative to lagged camera) |
| C-left/right | Orbit `camYaw` only |

Implementation notes match [conventions](/reference/conventions): 48 kHz audio,
box wall, snake `-yaw`, push/pop, `FILTERS_RESAMPLE`.

Also ships P1 polish from later lessons: **rumble** on collect (L34), **best clear
time** on EEPROM (L38), title/win HUD for best.

## Ranks

| Rank | Time |
|------|------|
| Gold | &lt; 45 s |
| Silver | &lt; 90 s |
| Bronze | otherwise |

Tune freely.

## Art swap

Replace `assets/*.t3dm` (and audio) in `capstone/starshard-cove/assets/` with your exports from Module 3. Keep names or update `t3d_model_load` paths.


## Full capstone source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`capstone/starshard-cove/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/capstone/starshard-cove/Makefile) · [`capstone/starshard-cove/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/capstone/starshard-cove/src/main.c)

::: details `capstone/starshard-cove/Makefile`
<<< ../../../capstone/starshard-cove/Makefile{make}
:::

::: details `capstone/starshard-cove/src/main.c`
<<< ../../../capstone/starshard-cove/src/main.c{c}
:::

## Next

[L36 — Shipping](./l36-shipping).
