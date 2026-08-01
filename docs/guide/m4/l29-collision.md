# L29 — Collision & world response

::: tip Goal
Detect **pickups** with simple sphere checks, keep the player on the island, respawn if they fall.
:::

## In plain English

We are not writing a physics engine.

| Check | Method |
|-------|--------|
| Collect shard | `distance_xz(player, shard) < radius` |
| Stay on island | Clamp radial distance |
| Fall | If `y < kill`, reset to spawn |

## What you will see

```bash
make -C lessons/l29-collision
```

Three bobbing starshards; walk into them to collect.

## Next

[L30 — Entities](./l30-entities).
