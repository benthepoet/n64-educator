# L34 — Juice & readability

::: tip Goal
Make pickups **feel** good: bob, spin, flash the clear color, play SFX on collect.
:::

## In plain English

Juice = feedback that says “your input mattered” without new systems:

| Trick | Implementation |
|-------|----------------|
| Bob/spin shards | `sin(time)` on Y + yaw |
| Collect flash | lerp clear color white for a few frames |
| SFX | `wav64_play` on collect |
| Anim speed | walk clip rate follows stick |

## What you will see

```bash
make -C lessons/l34-juice
```

Screen brightens briefly when you grab a shard.

## Performance note

Juice is free if it’s math + existing draws. Particles (Tiny3D examples) are optional stretch.

## Next

[L35 — Capstone assembly](./l35-capstone).
