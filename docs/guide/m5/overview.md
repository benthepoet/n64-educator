# Module 5 — Polish & Starshard Cove

::: tip Goal
Add **audio**, a clear **HUD**, **juice**, and ship the full **Starshard Cove** capstone (8 shards, ranks, title/pause/win).
:::

## Prerequisites

- Module 4 Toy Cove working  
- Tiny3D + mixer toolchain from earlier setup  

## Lessons

| Lesson | Focus | ROM |
|--------|--------|-----|
| [L32 — Audio](./l32-audio) | mixer, WAV SFX, XM music | `l32_audio` |
| [L33 — HUD](./l33-hud) | 2D text over 3D, timer | `l33_hud` |
| [L34 — Juice](./l34-juice) | flash, bob, feedback | `l34_juice` |
| [L35 — Capstone](./l35-capstone) | Assemble full game | `starshard_cove` |
| [L36 — Shipping](./l36-shipping) | Checklist, next steps | docs |

## Build

```bash
source scripts/env.sh
make l32 l33 l34
make -C capstone/starshard-cove
```

Play the finale:

```bash
make -C capstone/starshard-cove
# starshard_cove.z64 — START to play, collect 8, beat the clock for Gold
```

## Audio pipeline

```text
assets/*.wav → audioconv64 → filesystem/*.wav64
assets/*.xm  → audioconv64 → filesystem/*.xm64
```

Handled by `common/lesson.mk`. Call `mixer_try_play()` every frame.

**Must match Opus sample rate:** `audio_init(48000, …)` — see [L32](./l32-audio).

## Patterns that must match the ROMs

Same as Module 4 gameplay, plus:

| Topic | Course convention |
|-------|-------------------|
| Audio | `audio_init(48000)` + `wav64_init_compression(3)` for Opus |
| VI / clear | `FILTERS_RESAMPLE` + opaque clear |
| Camera / move | `camYaw` free orbit; move from lagged eye→player; box wall |
| Snake facing | `-yaw` in the model matrix |

[Scale & conventions](/reference/conventions) · [Module 4 patterns](/guide/m4/overview)

Start: [L32 — Audio](./l32-audio).
