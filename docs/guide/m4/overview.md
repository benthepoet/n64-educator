# Module 4 — Gameplay systems

::: tip Goal
Turn art + 3D rendering into a **small game**: move, animate, follow-cam, collide, spawn entities, and switch **title / play / pause / win**.
:::

## Prerequisites

- Modules 0–3 (especially Tiny3D load + asset lab)
- Comfortable reading multi-file C

## Lessons

| Lesson | Skill |
|--------|--------|
| [L26 — Move](./l26-move) | Camera-relative stick movement |
| [L27 — Anim drive](./l27-anim-drive) | Idle/walk blend from speed |
| [L28 — Follow cam](./l28-follow-cam) | Soft third-person camera |
| [L29 — Collision](./l29-collision) | Sphere pickups + bounds |
| [L30 — Entities](./l30-entities) | Spawn table + entity loop |
| [L31 — Game state](./l31-game-state) | Title / play / pause / win |
| [Toy Cove](./toy-cove) | Checkpoint mini-game |

## Shared helper

`common/include/ng_game.h` — deadzone, time, lerp, angle lerp, clamp.

## Patterns that must match the ROMs

| Topic | Course convention |
|-------|-------------------|
| Stick-up | Into the screen (look on XZ) — see L26 |
| Move basis (L28+) | Lagged **eye→player**, not player yaw; `camYaw` only from C-buttons |
| Soft wall | **Box** clamp (±5.5), not radial |
| Snake facing | Model yaw **`-yaw`** (mesh faces −Z) |
| Model matrices | `push` → draw → `pop` (not `matrix_set` over the camera) |
| Skinned player | `t3d_skeleton_use` then `t3d_model_draw_skinned` |
| Island / snake / shard scales | ~**0.032** / **0.02** / **0.02** on course `island` (L27 sample **map** uses different units) |
| VI / clear | `FILTERS_RESAMPLE` + opaque clear (`0xFF`) — no AA edge flicker |
| Audio (Module 5) | `audio_init(48000)` with Opus `wav64` |

Full checklist: [Scale & conventions](/reference/conventions).

## Build

```bash
source scripts/env.sh
make l26 l27 l28 l29 l30 l31
make -C lessons/m4-toy-cove
```

Start: [L26 — Move](./l26-move).
