# L11 — Tiny3D first quad

::: tip Goal
Get a **colored quad** spinning in 3D with Tiny3D: init, viewport (camera + lens), model matrix, vertices, lights, depth clear.
:::

## In plain English

Module 0 drew **pixels and sprites**. Module 1 explained **math**.  
Now the RSP runs Tiny3D microcode to transform triangles and the RDP shades them — a mini 3D engine you control from C.

This lesson is the 3D “hello world”: **four corners, two triangles**, spinning.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l11-t3d-quad
```

A multicolored quad tumbling in purple space, with a short HUD line.

---

## Frame recipe (memorize this shape)

Almost every Tiny3D frame looks like:

```text
1. Update camera + object matrices
2. rdpq_attach(color, depth)
3. t3d_frame_start()
4. t3d_viewport_attach(...)
5. clear color + clear depth
6. set lights + draw flags
7. draw meshes
8. optional 2D text
9. rdpq_detach_show()   ← same vsync-style present as L02
```

### Why a depth buffer?

`display_get_zbuf()` is a second image that stores **how near** each pixel is. When two triangles overlap on screen, the nearer one wins. L12 makes this obvious.

### Why `malloc_uncached` for matrices / verts?

The RSP **DMAs** that memory. Cached writes might not be visible to DMA yet. Uncached allocations avoid “I updated the matrix but the quad didn’t move” footguns.

---

## Pieces mapped to Module 1

| Code | Module 1 idea |
|------|----------------|
| `t3d_viewport_set_projection(fov, near, far)` | Projection / lens (L08) |
| `t3d_viewport_look_at(eye, target, up)` | View / camera (L08) |
| `fm_mat4_*` + `t3d_mat4_to_fixed` | Model matrix (L07) |
| `T3DVertPacked` + colors | Vertices + vertex color (L09) |
| ambient + directional | Lighting preview (L10) |

---

## Display list (optional speed trick)

The sample **records** the draw once (`rspq_block_begin/end`) and replays it. Inside
the recorded block the lesson uses **`t3d_matrix_push` → vert load → `t3d_matrix_pop`**
so T&L sees the model matrix; the matrix *memory* still updates each frame, so the
quad spins without rebuilding the list. You can also draw immediately without
recording — recording is an optimization pattern you’ll see often (and L15 needs
push/pop when recording a full model).

Before drawing, the lesson sets:

```c
rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
```

Without that, hand-built triangles often appear **black** even when vertex RGBA is set.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Black screen” | Forgot depth clear, camera inside geometry, or near/far wrong |
| “Black *mesh* on a colored clear” | Missing `RDPQ_COMBINER_SHADE` (or ambient almost zero) |
| “No 3D, flat mess” | Missing `t3d_viewport_attach` or projection |
| “Colors wrong” | Lighting multiplies vertex colors; ambient too dark |
| “Why uncached?” | RSP DMA vs CPU cache |

---

## Exercises

1. Change clear color.  
2. Slow `rotAngle += …`.  
3. Move `camPos` farther on Z — object looks smaller (perspective!).  


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l11-t3d-quad/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l11-t3d-quad/Makefile) · [`lessons/l11-t3d-quad/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l11-t3d-quad/src/main.c)

::: details `lessons/l11-t3d-quad/Makefile`
<<< ../../../lessons/l11-t3d-quad/Makefile{make}
:::

::: details `lessons/l11-t3d-quad/src/main.c`
<<< ../../../lessons/l11-t3d-quad/src/main.c{c}
:::

## What you learned

- Tiny3D init + per-frame attach/clear/draw/present  
- Viewport = projection + view  
- Uncached model matrix + packed vertices  

## Next

[L12 — Depth + objects](./l12-depth-objects) — two entities and why the Z-buffer matters.
