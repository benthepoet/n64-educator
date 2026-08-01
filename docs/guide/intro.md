# Introduction

Welcome to **N64 Educator** — a course for writing **3D games** for the Nintendo 64 using open-source tools.

## Who this is for

You should be comfortable with **C** (structs, pointers, headers, Make). You do **not** need:

- Prior game-engine experience
- Prior 3D math or graphics API knowledge
- Blender or art skills (we teach a minimal pipeline and ship reference assets)

If C is rusty, spend a weekend on a short refresher before Module 0.

## What you will build

By the end you will ship **Starshard Cove**: a small third-person collectathon on a single island. You walk an animated character, follow a camera, pick up starshards on a vertex-painted landscape, and hit a win screen with HUD and audio.

## Curriculum map

| Module | Focus |
|--------|--------|
| **0 — Foundations** | Toolchain, ROM, loop, input, assets, N64 mental model |
| **1 — 3D Literacy** | Vectors, matrices, cameras, meshes, lighting, vertex color concepts |
| **2 — Tiny3D** | First 3D draws, lights, models, vertex colors on console |
| **3 — Blender for N64** | Modeling, UVs, vertex paint, Fast64, rig + idle/walk |
| **4 — Gameplay** | Move, drive anims, camera, collision, entities, states |
| **5 — Capstone** | Audio, HUD, juice, full Starshard Cove (8 shards) |

## How to use this site

1. Complete [Setup](/guide/setup) once.
2. Work lessons in order under Module 0, then onward.
3. Each lesson has a folder under `lessons/` with a Makefile — build a `.z64` and run it in Ares.
4. Canonical assets live in `assets-src/`; you may replace them with your own art as you learn.

## Stack (high level)

- **libdragon** (`preview` branch) — modern N64 SDK in C
- **Tiny3D** — high-performance 3D pipeline (RSP + C API + glTF import)
- **Blender + Fast64** — models, vertex colors, materials, animation export
- **Ares** — accurate emulator for day-to-day development

See [Pinned versions](/reference/versions) before you install anything.

## Open source

Code is **MIT**, docs are **CC BY 4.0**, with third-party notices in `NOTICE` and
`assets-src/ATTRIBUTION.md`. See [License & credits](/reference/license).
Contributions: `CONTRIBUTING.md`.

Next: [Setup & toolchain](/guide/setup).
