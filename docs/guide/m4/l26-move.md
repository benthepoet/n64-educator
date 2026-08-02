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

Blockout **player_static** on the painted **island**. Stick moves; a **box** soft
wall (`ng_clamp` on X/Z) keeps you near the mesh.

### Course mesh scales (important)

`island.t3dm` stores large integer verts (~±384). The ROM uses **scale ≈ 0.032** so
the island is ~12 world units. `player_static` uses **scale ≈ 0.04** (~3–4u tall).
If you load these `.t3dm` files at scale `1,1,1`, the camera will look empty
(clear color + text only).

Display uses **`FILTERS_RESAMPLE`** (not AA) and an opaque clear so the top of
the frame does not flicker.

## Key idea: camera-relative

This lesson uses a **fixed** `camYaw` for the camera offset. Stick is rotated by
that yaw into world XZ. Stick-up should move **into the screen** (along look),
not toward the camera. L28 upgrades this to a free `camYaw` + lagged eye basis:

```text
// sx = stick right, sy = stick up (after deadzone)
mx =  sx * cos(θ) - sy * sin(θ)
mz = -sx * sin(θ) - sy * cos(θ)
position += (mx, mz) * speed * dt
```

If up/down feel inverted, the sign on `sy` is wrong (fixed in v1.1.2).

## Draw pattern

```c
t3d_matrix_push(islandMat);
t3d_model_draw(island);
t3d_matrix_pop(1);
t3d_matrix_push(&playerMat[frame]);
t3d_model_draw(player);
t3d_matrix_pop(1);
```

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
