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
| Rumble | `joypad_set_rumble_active` pulse (~0.18 s) if supported |
| Anim speed | walk clip rate follows stick |

## What you will see

```bash
make -C lessons/l34-juice
```

Screen brightens briefly when you grab a shard. Full loop: title → play (island +
snake + 3 shards) → audio/HUD → win.

Same draw/audio rules as earlier modules: **`audio_init(48000)`**, island/snake/
shard **scales**, **push/pop** matrices, **`t3d_skeleton_use`** before skinned
draw, **`FILTERS_RESAMPLE`** + opaque clear, free **`camYaw`** with move from
the lagged eye (not player yaw), box soft-wall, snake **`-yaw`**.

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

[L38 — Save & high scores](./l38-save) · then [L39](./l39-perf) · [L35 Capstone](./l35-capstone).
