# L31 — Game state machine

::: tip Goal
Structure the game as **states** so title, play, pause, and win don’t fight each other.
:::

## Diagram

```text
   START          START           3 shards
 TITLE ──► PLAY ⇄ PAUSE
             │
             └──► WIN ──START──► TITLE
```

Only **PLAY** runs movement and pickup logic. **PAUSE** freezes `dt` for anims.
**TITLE** slowly orbits the island for a showcase (still draws the full scene).

## What you will see

```bash
make -C lessons/l31-game-state
```

Title shows island + idle snake + shards + “Press START” (not a blank blue field).
Same render stack as L30: scales, `skeleton_use`, matrix push/pop, eye-relative
move in play, box soft-wall. Models load with `assertf` so a missing DFS asset
fails loudly.

Display: **`FILTERS_RESAMPLE`** + opaque clear.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l31-game-state/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l31-game-state/Makefile) · [`lessons/l31-game-state/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l31-game-state/src/main.c)

::: details `lessons/l31-game-state/Makefile`
<<< ../../../lessons/l31-game-state/Makefile{make}
:::

::: details `lessons/l31-game-state/src/main.c`
<<< ../../../lessons/l31-game-state/src/main.c{c}
:::

## Next

[Toy Cove checkpoint](./toy-cove).
