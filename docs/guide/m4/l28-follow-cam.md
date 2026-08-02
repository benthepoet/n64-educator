# L28 — Third-person follow camera

::: tip Goal
Place the camera **behind the player** with lag, and move relative to that view.
:::

## In plain English

```text
camYaw     = camera horizontal angle (C-left/right only — free orbit)
move       = rotate stick by camYaw into world XZ
player yaw = face the move direction (does NOT drive the camera basis)
desired_eye = player + offset(camYaw) * distance + up * height
eye/look   = lerp toward desired (positional lag)
```

### Why not couple camera to player yaw?

If movement is relative to **player yaw** and the camera sits on **player yaw**,
turning (strafe or forward facing updates) changes the move basis every frame →
the view **spins**. Same bug if you soft-lerp `camYaw` toward `playerYaw` while
running: those angles differ by ~π (camera sits behind, player faces the look
direction), so the lerp races around the circle.

**Rule:** move relative to **camYaw**; turn the **model** only; change **camYaw**
with C-buttons (or a dedicated free-look). Position lag still feels like a follow cam.

## What you will see

```bash
make -C lessons/l28-follow-cam
```

Camera trails with lag; stick-up = into the scene; C-left/right orbits; strafe does
not flip the camera.

Uses **skinned** `player_anim` (scale ~**0.02** on the course island) with
`t3d_skeleton_use` before `t3d_model_draw_skinned`, and **push/pop** for island
and player matrices.


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
