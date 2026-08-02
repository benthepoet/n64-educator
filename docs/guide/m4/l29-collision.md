# L29 — Collision & world response

::: tip Goal
Detect **pickups** with simple sphere checks, keep the player on the island, respawn if they fall.
:::

## In plain English

We are not writing a physics engine.

| Check | Method |
|-------|--------|
| Collect shard | `distance_xz(player, shard) < radius` |
| Stay on island | **Box** clamp on XZ (e.g. ±5.5) — not a radial cylinder |
| Fall | If `y < kill`, reset to spawn (pattern; bound is enough here) |

Radial “soft walls” skate you around the rim and feel like camera spin — use a
box like L28/L30.

## What you will see

```bash
make -C lessons/l29-collision
```

Three bobbing starshards; walk into them to collect. Same island/snake/shard
**scales**, **push/pop** draw pattern, **eye-relative move**, **`camYaw`**, and
snake **`-yaw`** as L28.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l29-collision/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l29-collision/Makefile) · [`lessons/l29-collision/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l29-collision/src/main.c)

::: details `lessons/l29-collision/Makefile`
<<< ../../../lessons/l29-collision/Makefile{make}
:::

::: details `lessons/l29-collision/src/main.c`
<<< ../../../lessons/l29-collision/src/main.c{c}
:::

## Next

[L30 — Entities](./l30-entities).
