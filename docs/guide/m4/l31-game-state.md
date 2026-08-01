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

## Next

[Toy Cove checkpoint](./toy-cove).
