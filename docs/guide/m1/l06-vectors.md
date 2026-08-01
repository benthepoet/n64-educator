# L06 — Space, points, vectors

::: tip Goal
Tell apart **points** and **vectors**, measure how long a vector is, turn it into a pure direction, and **add** two vectors. Your controller stick becomes a vector on screen.
:::

::: info Who this is for
If words like “normalize” sound scary: good, you’re in the right place. We’ll define every term with a story first.
:::

## Before you touch math

Imagine a paper map.

- A **point** is a pin in the map: “the treasure is *here*.”  
- A **vector** is an arrow: “walk 3 steps east and 1 step north” — it has **direction** and **length**, but it is not a place by itself.

You can put the same arrow anywhere: “3 east, 1 north” from home or from the dock is the same *instruction*, different *endpoints*.

Games are full of both:

| Situation | Point or vector? |
|-----------|------------------|
| Player position | Point |
| “Move left this frame” | Vector |
| Velocity / speed + direction | Vector |
| Collectible location | Point |
| “From me to the collectible” | Vector (point − point) |

## What you will see (ROM)

```bash
source scripts/env.sh
make -C lessons/l06-vectors
```

| On screen | Meaning |
|-----------|---------|
| Cross axes | Origin at screen center — our “zero” |
| Yellow-ish arrow | Your **vector** `v` from center |
| Numbers for `v` and `\|v\|` | Components and **length** |
| `dir = (...)` | Same direction, length forced to 1 |
| **C-right** | Toggle adding a fixed **wind** vector |

**Controls**

| Input | Effect |
|-------|--------|
| Stick | Change the free vector |
| C-right | Toggle `v = stick + wind` |

Play first: push the stick, watch the tip move, toggle wind and watch the tip **jump**. That jump *is* vector addition.

---

## Core ideas (slow)

### 1. Coordinates are just labeled rulers

In 2D we store two numbers: `(x, y)`.

```text
(0, 0)   origin
(10, 0)  ten units along +X
(0, 5)   five units along +Y
```

A **point** uses those numbers as “where.”  
A **vector** uses the same kind of numbers as “how much offset.”

In code they often look identical (`float x, y`) — the *meaning* is what differs. Naming helps: `position` vs `velocity`.

### 2. Point vs vector (table you can trust)

| | Point | Vector |
|---|--------|--------|
| Means | A location | A displacement / direction + amount |
| Example | “Player is at (3, 0, 5)” | “Move by (1, 0, 0)” this frame |
| From two points | — | `B - A` = arrow from A to B |
| Point + vector | New point | — |

**Recipe you’ll use forever:**

```text
new_position = old_position + velocity * dt
```

That’s “start at a point, add a vector.”

### 3. Length (magnitude)

How long is the arrow?

```text
|v| = sqrt(x*x + y*y)
```

- Stick centered → length ≈ 0  
- Stick pushed hard → length bigger  

On screen the ROM prints `|v|`. Deflect the stick fully and watch it grow.

::: tip Why care?
Speed is often “length of the velocity vector.” Distance to an enemy is length of `(enemy - me)`.
:::

### 4. Normalize = “keep direction, forget length”

Sometimes you only want **which way**, not how hard:

```text
dir = v / |v|     (when |v| is not zero)
```

Now `|dir|` is about **1**. You can say:

```text
velocity = dir * move_speed
```

So walking is always the same speed no matter how hard you smashed the stick (after you decide to ignore magnitude) — or you use length for analog speed. Either way, **normalize** is the tool for “unit direction.”

In the ROM, look at `dir = (...)` while you move the stick. The direction numbers change; their combined length stays ~1 when you’re not at zero.

::: warning Division by zero
If the stick is centered, length is 0 — you **must not** divide. Helpers return a zero vector instead. Same in real games: “if almost stopped, play idle; don’t normalize noise.”
:::

### 5. Adding vectors (the wind trick)

```text
result = stick + wind
```

Picture tip-to-tail: draw stick, then from its tip draw wind; the big arrow from start to final tip is the sum.

**C-right** toggles wind so you *feel* addition. Order doesn’t matter for plain addition (`a+b = b+a`).

---

## Deadzone (from L03, still matters)

Raw sticks jitter around zero. We ignore tiny values so “idle” is truly zero length — otherwise you’d normalize noise into a random direction. The ROM still uses a deadzone on the stick.

---

## Screen +Y vs world +Y (don’t get ambushed later)

| Space | Usual +Y |
|-------|----------|
| N64 framebuffer / many 2D UIs | **Down** the screen |
| 3D world (this course) | **Up** |

The L06 demo **negates** stick Y so “stick up” feels like up on screen. In true 3D levels, +Y is up in world space. When UI and world math mix, be explicit about which space you’re in.

---

## Course helpers (you don’t write the sqrt yourself)

```c
#include "ng_math.h"

ng_vec2 v = ng_v2(x, y);
float len = ng_v2_len(v);
ng_vec2 dir = ng_v2_normalize(v);
ng_vec2 sum = ng_v2_add(a, b);
```

Same ideas exist as `ng_vec3` for 3D (`x, y, z`).

---

## Walk the source (optional)

Open `lessons/l06-vectors/src/main.c`:

1. **Update** — read stick → build `ng_vec2` → optional wind → length / normalize.  
2. **Render** — draw axes, draw line from center to tip, print numbers.  

If the file feels long, ignore drawing helpers (`draw_line`) and only read the block that builds `v` and `dir`.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Points and vectors are the same in C” | Same storage, different **meaning** — name them clearly |
| “Normalize broke my speed” | You removed length on purpose; multiply by a speed after |
| “Wind made it jump” | That’s addition working — good! |
| “Why is dir (0,0) at rest?” | Length was ~0; we refuse to normalize |

---

## Exercises

1. Push the stick and confirm `|dir|` stays near **1** when not centered.  
2. Change wind’s numbers in source; rebuild; feel the new pull.  
3. (Stretch) On paper: point A=(1,1), B=(4,5). What vector goes from A to B? What’s its length?


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l06-vectors/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l06-vectors/Makefile) · [`lessons/l06-vectors/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l06-vectors/src/main.c)

::: details `lessons/l06-vectors/Makefile`
<<< ../../../lessons/l06-vectors/Makefile{make}
:::

::: details `lessons/l06-vectors/src/main.c`
<<< ../../../lessons/l06-vectors/src/main.c{c}
:::

## What you learned

- Point = where; vector = how much / which way  
- Length and normalize  
- Adding vectors  
- Deadzone + “don’t normalize zero”

## Next

[L07 — Matrices](./l07-matrices) — how we move, spin, and resize whole objects without editing every point by hand.
