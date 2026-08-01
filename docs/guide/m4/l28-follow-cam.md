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

Camera trails; movement stays camera-relative including orbit.

## Next

[L29 — Collision](./l29-collision).
