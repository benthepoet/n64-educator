# L07 — Matrices & transforms

::: tip Goal
Learn that **translate**, **rotate**, and **scale** can be stored as **matrices**, combined into one **model matrix**, and used to place local geometry into the world — without rewriting every corner by hand.
:::

::: info Scary word check
A **matrix** here is just a grid of numbers the computer multiplies with a point to get a new point. You do **not** need to multiply 4×4 grids on paper. We use helpers.
:::

## The everyday problem

Your square’s corners are defined once, nicely centered at the origin:

```text
(-0.5, -0.5), (0.5, -0.5), (0.5, 0.5), (-0.5, 0.5)
```

In the game you want that square:

- **Somewhere else** (translate)  
- **Spun** (rotate)  
- **Bigger or smaller** (scale)  

You could write special code for each corner every frame. Instead we build one transform **M** and do:

```text
world_point = M × local_point
```

for each corner. Same idea later for thousands of mesh vertices.

## What you will see (ROM)

```bash
source scripts/env.sh
make -C lessons/l07-matrices
```

A square you can **move**, **spin**, and **resize**. The white dot is the translation pivot (where the object’s origin sits in the world).

| Input | Effect |
|-------|--------|
| Stick | Translate (move the square) |
| L / R | Rotate |
| C-up / C-down | Scale up / down |

---

## Three basic transforms (stories)

### Translate — “put it over there”

Add an offset to every point: “same shape, new place.”

### Rotate — “spin around the origin”

In this lesson we rotate around **Z** (the axis sticking out of the 2D screen). The square turns in place around its local center *before* we slide it away — because of composition order (below).

### Scale — “make it bigger/smaller”

Multiply coordinates by a size. Scale of 40 means our 1×1 local square becomes 40×40 pixels-ish on screen.

---

## Why call it a matrix?

Hardware and engines (Tiny3D included) almost always want a **4×4 matrix** for object placement. One matrix can hold rotate + scale + translate together. One multiply per point is a regular, optimizable pattern.

You can think:

```text
matrix = a reusable machine
input  = local point
output = world point
```

### Column-major, M × v (course convention)

We store matrices **column-major** and multiply **matrix × vector**. You only need to remember:

- Helpers like `ng_mat4_mul_point` do the multiply.  
- When we **compose** matrices, order matters a lot.

---

## Composition: T × R × S (read right-to-left on the point)

The ROM builds:

```text
M = T × R × S
```

For a point, apply **right-most first**:

1. **S** — scale in local space  
2. **R** — rotate around local origin  
3. **T** — translate into the world  

Story: “Size the toy, spin the toy, then place the toy on the table.”

::: warning Order matters a lot
`T × R` ≠ `R × T`.

- Rotate **then** move: object spins around its center, then slides.  
- Move **then** rotate: object can **orbit** around the world origin like a planet.

If something “orbits when I only wanted it to spin,” your order or pivot is wrong. This bites everyone once.
:::

### Local vs world (two coordinate systems)

| Space | Plain English |
|-------|----------------|
| **Local / model** | Coordinates as you authored the shape (center at 0 is nice) |
| **World** | Coordinates in the level — after the model matrix |

Art assets almost always live in local space. Gameplay asks “where in the level?” → world space.

---

## The bigger pipeline (preview only)

Eventually:

```text
local  --M-->  world  --V-->  view  --P-->  clip/screen
         model         camera        lens
```

- **L07** = **M** (model)  
- **L08** = **V** (view / camera)  
- Module 2 = **P** for real on the 3D hardware  

You don’t need all of this today — just know M is only the first stage.

---

## Course API (copy-paste level)

```c
ng_mat4 M;
ng_mat4_trs_z(&M,
    ng_v3(tx, ty, 0.f),   /* translate */
    rot_radians,          /* rotate about Z */
    ng_v3(s, s, 1.f));    /* scale */

ng_vec3 world = ng_mat4_mul_point(&M, local);
```

`ng_mat4_trs_z` builds that **T × R × S** for 2D-ish demos.

---

## Walk the ROM mentally

1. Four **local** corners of a unit square.  
2. Each frame, read stick / shoulders → update `tx, ty, rot, scale`.  
3. Build **M**.  
4. Transform each corner to world.  
5. Draw edges between world corners.  

You’re not “drawing a matrix.” You’re drawing points that the matrix moved for you.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “I must learn to multiply matrices by hand” | No — understand **what** T/R/S do and **order** |
| “Scale then rotate vs rotate then scale” | Non-uniform scale + rotate can shear; we use uniform scale |
| “Pivot is wrong” | Rotation is around local origin (0,0); center your mesh on origin in Blender later |
| “Matrix is 4×4 but we’re in 2D” | 3D APIs still use 4×4; z=0 is fine for teaching |

---

## Exercises

1. Only use stick (no L/R). Can you place the square in each screen corner?  
2. Spin with L/R while scale is large — notice rotation is around the white center.  
3. Read the one-line call to `ng_mat4_trs_z` in `main.c` and match args to T / R / S.  
4. (Stretch) Draw a second square with a fixed offset in local space (a “child” mental model).

## What you learned

- Model matrix places local geometry in the world  
- Translate, rotate, scale as stories + one combined M  
- Composition order: **S then R then T** for our TRS helper  
- Local vs world  

## Next

[L08 — Camera](./l08-camera) — moving the *eyes*, not only the objects.
