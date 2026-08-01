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

Only **PLAY** runs movement and pickup logic. **PAUSE** freezes `dt` for anims. **TITLE** orbits the island.

## What you will see

```bash
make -C lessons/l31-game-state
```


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
