# Module 1 — 3D Literacy

::: tip Goal
Build the **mental models** for 3D games *before* Tiny3D. When Module 2 throws matrices and lights at you, you should think “oh, that’s just L07 / L10,” not “what is this alien language?”
:::

## If you’ve never done 3D before

That’s expected. This module assumes:

- You finished Module 0 (you can build a ROM, read the stick, understand the game loop).
- You can read simple C (`structs`, function calls, `float`).

It does **not** assume high-school trig fluency, linear algebra, or engine experience. When math appears, we use:

1. A plain-English story  
2. A picture / table  
3. A tiny formula only if it helps  
4. A ROM you can poke with the controller  

You will **not** hand-derive 4×4 inverse matrices. The course ships `ng_math` helpers so you can *use* ideas while learning them.

## Why this module exists

Jumping straight into Tiny3D is like learning to cook by only reading a restaurant’s order tickets. You’ll copy code, but you won’t know *why* the camera moves “wrong” or why a model is “inside out.”

Module 1 is the vocabulary:

| Everyday idea | 3D name | Lesson |
|---------------|---------|--------|
| “Where am I?” vs “which way / how far?” | Point vs vector | L06 |
| Move, spin, resize an object | Model matrix (T/R/S) | L07 |
| “The world as seen from my eyes” | View / camera | L08 |
| “The shape is made of triangles” | Mesh / vertices | L09 |
| “Paint and light the surface” | Color, ambient, vertex tint | L10 |

## How to study each lesson

1. **Read the “in plain English” section** first — skip code if you’re overwhelmed.  
2. **Build and run the ROM** — play with controls listed on screen.  
3. **Come back** to the math/API bits once you’ve *seen* the effect.  
4. Do at least one **exercise** (even a tiny one).  

::: info Don’t binge without running ROMs
3D clicks when your thumbs move something on screen. Reading alone is half the lesson.
:::

## Lessons at a glance

| Lesson | Idea | What you’ll do in the ROM |
|--------|------|---------------------------|
| [L06 — Vectors](./l06-vectors) | Points vs vectors, length, normalize, add | Stick draws a vector; add “wind” |
| [L07 — Matrices](./l07-matrices) | Translate / rotate / scale | Drag, spin, and resize a square |
| [L08 — Camera](./l08-camera) | View transform; lens intuition | Pan a camera over a tiny world |
| [L09 — Meshes](./l09-meshes) | Triangles, winding, vertex color | Spin a rainbow triangle |
| [L10 — Color & light](./l10-color-light) | Ambient, albedo, **vertex blend** | Compare flat vs painted surface |

## Shared code (`ng_math`)

Lessons use:

- `common/include/ng_math.h`  
- `common/src/ng_math.c`  

A lesson Makefile turns this on with:

```make
USE_NG_MATH := 1
```

You do **not** need to understand every line of `ng_math.c`. Treat it like `libdragon`: call the helpers, read the header comments when curious.

## Conventions (one place to remember)

See also [Scale & conventions](/reference/conventions).

| Topic | Course choice |
|-------|----------------|
| World “up” | **+Y** |
| Matrix layout | **Column-major** |
| Transform a point | **M × v** (matrix times vector) |
| Model compose | Often **T × R × S** (scale, then rotate, then translate — see L07) |

Pixel screens often have **+Y down**. World 3D has **+Y up**. Demos call this out when it matters — don’t panic if stick “up” is negated in 2D UI code.

## What Module 1 is *not*

- Not Tiny3D setup (that’s Module 2)  
- Not Blender (Module 3)  
- Not a full math textbook  
- Not “memorize every formula”

## Comfort check before you start

You should be able to:

- [ ] Build and run L02 / L03 in Ares  
- [ ] Explain update vs render in one sentence  
- [ ] Read a `struct` with a few `float` fields  

If C itself is shaky, pause and refresh C before this module. 3D is enough new ideas on its own.

## Build all Module 1 ROMs

```bash
source scripts/env.sh
make l06
make l07
make l08
make l09
make l10
```

Start here: [L06 — Vectors](./l06-vectors). Go slow. Confused is normal; stuck for an hour without running the ROM is not — run the ROM.
