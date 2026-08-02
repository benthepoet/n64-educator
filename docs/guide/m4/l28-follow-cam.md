# L28 — Third-person follow camera

::: tip Goal
Drive a **follow camera** with its **own yaw** and positional lag, and move relative to **what you see** — not player facing.
:::

## In plain English

```text
camYaw     = camera horizontal angle (C-left/right only — free orbit)
eyeWant    = player + offset(camYaw) * distance + up * height
eye/look   = lerp toward desired (positional lag)

move basis = direction of (eye → player) on XZ   // lagged view, not player yaw
             fallback: camYaw if eye sits on the player
player yaw = face the move direction (model only — never drives the camera)
```

### Why not couple camera to player yaw?

If movement is relative to **player yaw** and the camera sits on **player yaw**,
turning (strafe or forward facing updates) changes the move basis every frame →
the view **spins**. Same bug if you soft-lerp `camYaw` toward `playerYaw` while
running: those angles differ by ~π (camera sits behind, player faces the look
direction), so the lerp races around the circle.

**Rule:**

1. **camYaw** only from C-buttons (desired orbit).  
2. **Move** relative to the **lagged eye** (what is on screen), not player yaw.  
3. **Player yaw** turns the model only.  
4. **Never** soft-follow `camYaw` → player yaw.

## What you will see

```bash
make -C lessons/l28-follow-cam
```

Camera trails with lag; stick-up = into the scene; C-left/right orbits; strafe does
not flip the camera.

Uses **skinned** `player_anim` (scale ~**0.02** on the course island) with
`t3d_skeleton_use` before `t3d_model_draw_skinned`, and **push/pop** for island
and player matrices. Snake model yaw is **`-yaw`** (mesh faces −Z). Soft wall is
a **box** clamp.

Display: **`FILTERS_RESAMPLE`** + opaque clear (no VI AA edge flicker).


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l28-follow-cam/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l28-follow-cam/Makefile) · [`lessons/l28-follow-cam/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l28-follow-cam/src/main.c)

::: details `lessons/l28-follow-cam/Makefile`
<<< ../../../lessons/l28-follow-cam/Makefile{make}
:::

::: details `lessons/l28-follow-cam/src/main.c`
<<< ../../../lessons/l28-follow-cam/src/main.c{c}
:::

## Next

[L29 — Collision](./l29-collision).
