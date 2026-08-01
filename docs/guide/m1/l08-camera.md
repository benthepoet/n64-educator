# L08 — Camera & projection

::: tip Goal
See the **camera** as “where my eyes are,” not as a mesh you draw. Learn the **view** transform (world → camera-relative) and get intuition for **projection** (the lens) before real 3D hardware.
:::

## A story that unlocks the whole lesson

You’re in a museum (the **world**). Statues stand still.

When **you walk left**, the statues appear to slide **right** on your retina. The statues didn’t move — **you** did.

Games do the same: we rarely animate the whole world backwards. We store a camera pose and compute:

```text
“Where is this world point, relative to my eyes?”
```

That relative position is **view space**. Drawing uses that.

## What you will see (ROM)

```bash
source scripts/env.sh
make -C lessons/l08-camera
```

| Thing | Meaning |
|-------|---------|
| Colored dots | Props fixed in **world** space |
| **Red** dot | World origin `(0,0)` |
| White reticle | Screen center = “where the camera is looking through” |
| Stick | Moves the **camera**, not the props |
| **A** | Toggle zoom (cheap stand-in for lens / FOV feel) |

::: tip Try this
Push stick left. Props slide right. That’s the museum story. You’re not failing math — the view transform is working.
:::

---

## View transform (2D version in this ROM)

We use a simple orthographic idea:

```text
screen = (world - camera) * zoom + screen_center
```

| Piece | Meaning |
|-------|---------|
| `world - camera` | “Where is the prop relative to my eye?” |
| `* zoom` | Bigger zoom → world looks larger (telephoto vibes) |
| `+ screen_center` | Draw relative to the middle of the TV |

This is the **translation part** of a view matrix. Full 3D also rotates so “camera forward” matches looking down a standard axis; see `ng_mat4_look_at` later — same idea, more components.

::: info You don’t draw the camera
The camera is numbers (position, orientation, lens). You draw **world stuff transformed into view/screen**. If you need a “camera mesh” for a cutscene, that’s just another model — not the mathematical camera.
:::

---

## Model vs view (tie to L07)

| Matrix | Answers |
|--------|---------|
| **Model (M)** | Where is *this object* in the world? |
| **View (V)** | How does the world look from the *camera*? |

Object path:

```text
local --M--> world --V--> view
```

Gameplay often moves either the player model, the camera, or both (third-person follow = camera depends on player).

---

## Projection — the “lens” (intuition only)

So far L08 is basically a **flat** camera (orthographic-ish): no vanishing points.

| Kind | Feel | Examples |
|------|------|----------|
| **Orthographic** | Distance doesn’t shrink objects; parallel lines stay parallel | Top-down strategy, many 2D games, UI |
| **Perspective** | Far things look smaller; has FOV | Mario 64, most 3D games |

Perspective needs extra work:

- **FOV** — field of view (wide angle vs zoom)  
- **Near / far planes** — only draw a depth slice (too near or too far = clipped)  
- A **projection matrix P**  

Tiny3D Module 2 will set FOV/near/far on a **viewport**. Your job is choosing values that match your world scale (if near/far are wrong, things vanish or z-fight).

### Frustum (vocab)

The visible volume of a perspective camera looks like a pyramid with the top cut off — the **view frustum**. “Outside the frustum” ≈ off-screen or clipped.

---

## Full chain (map for later)

```text
local  --M-->  world  --V-->  view  --P-->  clip  -->  hardware/screen
         model         camera        lens
```

| Stage | Lesson |
|-------|--------|
| M | L07 |
| V | L08 (2D now, 3D in Module 2) |
| P | Module 2 Tiny3D viewport |

---

## Why moving the camera feels “inverted”

Students often say: “I added to camera.x but the world went the wrong way.”

Remember:

```text
view = world - camera
```

If camera.x **increases**, `world - camera` **decreases** → props shift left on screen when you “move right,” depending on axis signs. Match the museum story, not gut panic.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “I should move every object opposite the stick” | That’s emulating a camera the hard way; use a camera offset instead |
| “Zoom is FOV” | Related idea, not identical math — good enough intuition for now |
| “Camera is an object at the reticle” | Reticle is screen center; camera is a world pose |
| “look_at is magic” | It builds a view matrix from eye, target, and up — readable in `ng_math.c` when ready |

---

## Exercises

1. Move until the **red** origin sits under the white reticle. What’s your camera position roughly?  
2. Toggle zoom with **A** without moving — props grow/shrink around the view.  
3. Explain to a rubber duck why stick-left makes props slide right.  
4. (Stretch) Skim `ng_mat4_look_at` comments/code — identify eye, target, up.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l08-camera/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l08-camera/Makefile) · [`lessons/l08-camera/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l08-camera/src/main.c)

::: details `lessons/l08-camera/Makefile`
<<< ../../../lessons/l08-camera/Makefile{make}
:::

::: details `lessons/l08-camera/src/main.c`
<<< ../../../lessons/l08-camera/src/main.c{c}
:::

## What you learned

- Camera = pose + lens, not a required mesh  
- View space = world relative to camera  
- Perspective vs ortho at a gut level  
- Where V sits between model and projection  

## Next

[L09 — Meshes](./l09-meshes) — what we actually draw: triangles and the data glued to their corners.
