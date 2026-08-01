# L26 — Player movement in 3D

::: tip Goal
Move a character on the island with the stick, **relative to the camera**, and face the movement direction.
:::

## In plain English

Stick left/right/up/down should feel like “strafe / forward” based on where the camera looks — not raw world axes. We:

1. Read stick as a 2D vector  
2. Rotate it by camera yaw into world XZ  
3. Integrate position with `dt`  
4. Slerp/lerp **yaw** toward the move direction  

## What you will see

```bash
make -C lessons/l26-move
```

Static player on painted island. Stick moves; soft wall keeps you on the mesh-ish disk.

## Key idea: camera-relative

The follow camera sits on an offset `(sin θ, cos θ)` looking at the player, so
**look** on XZ is `(-sin θ, -cos θ)`. Stick-up should move **into the screen**
(along look), not toward the camera:

```text
// sx = stick right, sy = stick up (after deadzone)
mx =  sx * cos(θ) - sy * sin(θ)
mz = -sx * sin(θ) - sy * cos(θ)
position += (mx, mz) * speed * dt
```

If up/down feel inverted, the sign on `sy` is wrong (that was a real course bug
fixed in v1.1.2).

## Exercises

1. Change `moveSpeed`.  
2. Print yaw in degrees.  
3. (Stretch) Press C-left/right to rotate `camYaw` (L28 does better follow).


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l26-move/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l26-move/Makefile) · [`lessons/l26-move/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l26-move/src/main.c)

::: details `lessons/l26-move/Makefile`
<<< ../../../lessons/l26-move/Makefile{make}
:::

::: details `lessons/l26-move/src/main.c`
<<< ../../../lessons/l26-move/src/main.c{c}
:::

## Next

[L27 — Anim drive](./l27-anim-drive).
