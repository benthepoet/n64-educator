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


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l34-juice/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l34-juice/Makefile) · [`lessons/l34-juice/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l34-juice/src/main.c)

::: details `lessons/l34-juice/Makefile`
<<< ../../../lessons/l34-juice/Makefile{make}
:::

::: details `lessons/l34-juice/src/main.c`
<<< ../../../lessons/l34-juice/src/main.c{c}
:::

## Next

[L35 — Capstone assembly](./l35-capstone).
