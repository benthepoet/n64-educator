# L13 — Orbit camera in 3D

::: tip Goal
Control a **3D camera** with the stick: orbit yaw/pitch and dolly distance. Connect L08’s museum story to `look_at`.
:::

## In plain English

L08 moved a 2D camera with `world - cam`.  
Here the camera sits on a **sphere** around the origin:

```text
eye.x = dist * cos(pitch) * sin(yaw)
eye.y = dist * sin(pitch)
eye.z = dist * cos(pitch) * cos(yaw)
```

Then:

```c
t3d_viewport_look_at(&viewport, &eye, &target, up);
```

builds the **view** matrix (Module 1 “V”).

## What you will see

```bash
source scripts/env.sh
make -C lessons/l13-orbit-camera
```

Green ground quad + red marker. HUD shows yaw/pitch/dist.

| Input | Effect |
|-------|--------|
| Stick X | Orbit yaw |
| Stick Y | Orbit pitch (clamped so you don’t flip under the floor) |
| C-up / C-down | Dolly closer / farther |

---

## Third-person preview

Starshard Cove’s follow camera is “orbit-ish” but **target = player**, and distance/angles are smoothed (L23 later). This lesson is the raw spatial skill.

---

## Near / far / FOV reminders

`t3d_viewport_set_projection(fov, near, far)`:

- **near** too large → close ground disappears  
- **far** too small → horizon clips  
- **FOV** wider → more scene, more distortion  

Units should match your world scale (course: ~1 unit ≈ 1 meter-ish).

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Pitch inverted” | Stick Y sign is a preference; change the `+=` sign if you hate it |
| “I fell under the world” | Pitch clamp — keep a minimum angle |
| “look_at is a black box” | Eye, target, up → view matrix; same as L08 + rotation |

---

## Exercises

1. Orbit until you view the ground edge-on — notice perspective.  
2. Dolly in until `near` almost clips; then ease out.  
3. (Stretch) Make target `(0, 4, 0)` so you orbit the red marker.

## What you learned

- Spherical orbit camera  
- `look_at` as view matrix  
- FOV/near/far as practical knobs  

## Next

[L14 — Lighting](./l14-lighting) — ambient + sun you can steer.
