# L09 — Meshes: vertices, triangles, winding

::: tip Goal
See 3D (and 2D) shapes as **triangles** made from **vertices**. Each vertex can carry extra data (especially **color**). Learn why **winding order** is a real thing, not pedantry.
:::

## If “mesh” is a new word

A **mesh** is the drawable shape: a bunch of corners connected into triangles.

- A triangle has **3 corners**.  
- A rectangle is **2 triangles**.  
- A low-poly character might be hundreds or thousands of triangles.  

GPUs (and the N64 RDP path we’re aiming at) are built around triangles. Almost every 3D model is “triangles all the way down.”

## What you will see (ROM)

```bash
source scripts/env.sh
make -C lessons/l09-meshes
```

| What | Why it’s there |
|------|----------------|
| One triangle | Smallest interesting mesh |
| Red / green / blue corners | **Per-vertex colors** |
| Colors blending in the middle | Hardware **interpolates** across the triangle (Gouraud shading) |
| Stick X | Rotate |
| **A** | Flip **winding** (swap two corners’ order) |

Stare at the middle of the triangle: you don’t store a color for every pixel; the GPU blends corner colors. That’s the same mechanism vertex lighting and vertex paint will use.

---

## Anatomy (build-up)

### Vertex

A **vertex** is a corner. At minimum: a **position**. Often also:

| Attribute | Newbie meaning |
|-----------|----------------|
| **Position** | Where is this corner? |
| **Color** | What color is painted on this corner? |
| **Normal** | Which way does the surface face? (for lighting — Module 2) |
| **UV** | Which pixel of a texture maps here? (Module 2–3) |

### Triangle

Three vertices (or three **indices** into a vertex list). One filled polygon.

### Mesh

Many triangles, usually sharing vertices so edges match.

```text
Vertex  = position + attributes
Triangle = 3 vertices
Mesh     = many triangles
```

---

## Vertex color (first meeting)

Instead of one color for the whole triangle, each corner can have its own RGBA. The rasterizer blends between them.

Why care on N64?

- Cheap detail (warm vs cool side of a rock)  
- Later: **paint the terrain** in Blender and multiply with a tiling texture (L10 + Module 3)  

L09 shows the blend with loud RGB so you can’t miss it.

---

## Winding order (the “why is my triangle gone?” topic)

List the three corners in a consistent order when viewed from the **front** — e.g. counter-clockwise.

```text
  0
 / \
1---2     order 0→1→2 might be CCW (example)
```

Engines can **cull** (throw away) triangles that face away from the camera, using winding to decide “front” vs “back.” That saves drawing leaves of a tree from the inside, etc.

**A** in the ROM swaps two vertices. You’re not changing positions of the shape’s idea — you’re changing **order**. On some setups the triangle would disappear when back-facing; here we still draw it, but you practice the idea that order is data.

::: tip Authoring tip (future you)
In Blender, “recalculate normals outside” and consistent normals are cousins of this idea. Inside-out meshes often look dark or missing faces.
:::

---

## Indexed meshes (why games don’t duplicate everything)

Suppose a quad (two triangles, four corners):

```text
vertices:  v0, v1, v2, v3
indices:   0,1,2,  0,2,3
```

Both triangles share `v0` and `v2`. Benefits:

- Less memory  
- Shared attributes stay seamless (same color/normal at the shared edge)  

When you *want* a hard crease, artists **split** vertices (two verts in the same place, different normals).

---

## How this ROM cheats (honest teaching)

We draw with libdragon RDP **screen-space** shaded triangles (`TRIFMT_SHADE`), and we transform corners with `ng_math` ourselves.

That’s intentional:

- You see vertices + colors **now**  
- Module 2 loads real Tiny3D meshes and lets the RSP help  

Same concepts; different pipeline maturity.

---

## Common noob confusions

| Feeling | Reality |
|---------|---------|
| “Meshes are files only” | A file is stored triangles; in RAM it’s still verts + tris |
| “Smooth color means a texture” | Not necessarily — vertex color alone can blend |
| “Winding is random” | It’s a deliberate front-face convention |
| “One triangle isn’t a mesh” | It’s the smallest mesh; scale up from here |

---

## Exercises

1. Change the three corner colors in `main.c`; rebuild.  
2. Hold a mental image: cube = 6 faces × 2 tris = 12 triangles (more if smoothed/split).  
3. On paper, square corners 0–3; write indices for two triangles.  
4. Press **A** a few times — predict what “flip winding” did to the vertex list.

## What you learned

- Mesh = triangles built from vertices  
- Attributes ride on vertices (color first)  
- Colors interpolate across a triangle  
- Winding marks front vs back  

## Next

[L10 — Color & light](./l10-color-light) — ambient light, base color, and why **multiply by vertex color** is an N64 superpower.
