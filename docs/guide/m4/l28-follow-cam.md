# L28 — Third-person follow camera

::: tip Goal
Place the camera **behind the player** with lag, and move relative to that view.
:::

## In plain English

```text
desired_eye = player_pos - forward * distance + up * height
eye = lerp(eye, desired_eye, lag)
look = lerp(look, player_pos + head_offset, lag)
```

C-left/right adds an **orbit** offset so you can peek.

## What you will see

```bash
make -C lessons/l28-follow-cam
```

Camera trails; movement stays camera-relative including orbit (stick-up = into
scene, same math as L26).

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
