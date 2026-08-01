# L05 — N64 hardware tour

::: tip Goal
Build a friendly mental model of the N64 so later 3D and performance talk is not magic. **No assembly required.**
:::

You have already used several pieces of the machine without naming them. This lesson pins the labels.

## The cast of characters

```text
┌─────────────────────────────────────────────────────────┐
│                     Nintendo 64                          │
│                                                         │
│   ┌──────────┐     commands      ┌──────────────────┐  │
│   │   CPU    │ ───────────────► │   RCP             │  │
│   │ R4300i   │                  │  ┌─────┐ ┌─────┐  │  │
│   │ (game    │ ◄── DMA/status ──│  │ RSP │ │ RDP │  │  │
│   │  logic)  │                  │  └─────┘ └─────┘  │  │
│   └────┬─────┘                  └─────────┬────────┘  │
│        │                                  │           │
│        ▼                                  ▼           │
│   ┌──────────────────────────────────────────────┐    │
│   │              RDRAM  (main RAM)                │    │
│   │   code · framebuffers · audio · meshes …     │    │
│   └───────────────────────┬──────────────────────┘    │
│                           │ scan out                   │
│                           ▼                            │
│                      ┌────────┐                        │
│                      │   VI   │ → TV / emulator       │
│                      └────────┘                        │
└─────────────────────────────────────────────────────────┘
```

| Chip / unit | Role in plain language | You already touched it via… |
|-------------|------------------------|-----------------------------|
| **CPU** (R4300i) | Runs your C code: game loop, input, AI | Everything in `main.c` |
| **RCP** | Reality Co-Processor — graphics/audio house | RDPQ / display stack |
| **RSP** | Vector DSP-like unit: geometry, audio mix, custom microcode | Tiny3D later; mixer under the hood |
| **RDP** | Triangle/raster/texture hardware | `rdpq_clear`, `rdpq_sprite_blit`, text |
| **RDRAM** | Shared main memory (~4–8 MiB depending on system) | Framebuffers, sprites, heaps |
| **VI** | Video Interface — scans framebuffers to the display | [L02 vsync / present](/guide/m0/l02-game-loop) |
| **Cartridge ROM** | Your `.z64` program + DFS assets | `rom:/` loads in L04 |
| **Joybus** | Controllers / paks | `joypad_*` in L03 |

## TMEM — the 4 KiB texture budget

The RDP does **not** sample giant textures straight from RDRAM every pixel without loading them first. Textures are staged into **TMEM** (texture memory), only about **4 KiB**.

Consequences you will feel later:

- Prefer **small**, power-of-two textures (32×32, 64×64)
- Formats and compression matter (CI4/CI8 palettes, etc.)
- Vertex **colors** (Module 1–3) tint large areas without more unique texels — classic N64 trick

L04’s 32×32 star is intentionally tiny.

## Frame budget intuition

NTSC is about **60 frames per second** → roughly **16.6 ms** per frame for *everything* (CPU + RSP + RDP). PAL ~50 Hz → ~20 ms.

If work overruns:

- Animation and input still *feel* tied to `display_get` pacing, but you may **drop** visual smoothness
- 3D scenes that are too heavy hitch; poly and texture thrash show up as RDP/RSP cost

You do not need to profile yet — just remember: **the TV clock is the boss** (L02).

## How libdragon maps onto this

| Your call | Hardware story |
|-----------|----------------|
| C code in the loop | CPU |
| `joypad_poll` | CPU talks to Joybus / SI |
| `rdpq_*` drawing | CPU records commands → RSP/RDP execute |
| `display_get` / show | VI + framebuffer ownership (vsync-style present) |
| `sprite_load("rom:/…")` | DMA/read from cartridge DFS into RDRAM |
| Tiny3D (later) | RSP microcode + RDP for 3D |

## Resolution choice in this course

We use **320×240**, 16 bpp, double buffer often. Higher resolutions cost more RDRAM for framebuffers and more RDP fill rate. Start modest; scale up when you understand the budget.

## What this lesson is *not*

- MIPS assembly or RSP programming  
- Exact DMA programming  
- A full datasheet  

Those are optional deep dives after you ship *Starshard Cove*.

## Checkpoint

Before Module 1, complete the [Module 0 checkpoint](/guide/m0/checkpoint): count with **A**, reset with **B**, draw DFS stars. That proves environment + loop + input + assets.

## What you learned

- CPU vs RCP (RSP + RDP) vs VI vs RDRAM vs cartridge  
- Why TMEM and small textures matter  
- Frame time is real and vsync-paced  
- How your libdragon calls map onto chips  

## Next

Module 1 — [3D Literacy](/guide/m1/overview) (vectors, matrices, cameras, meshes, vertex color) with interactive ROMs and `ng_math` helpers — still no Tiny3D yet.
