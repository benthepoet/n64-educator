# L20 — Vertex paint & color layers

::: tip Goal
Paint **per-vertex colors** for terrain tints. Name layers the pipeline expects (**`Col`**, optional **`Alpha`**).
:::

## Why (again)

Module 1–2: multiply texture × vertex color for cheap dirt paths and biome shifts. Now you author that paint.

## Steps

1. Select mesh → **Vertex Paint** mode (mode menu).  
2. Ensure a color attribute exists named **`Col`** (Data / Color Attributes panel).  
3. Brush soft falloff; paint grass vs path.  
4. For Fast64, you may also need **`Alpha`**.  

## Make glTF export keep colors

Vanilla Blender only exports vertex colors **if the material uses them** (or certain export flags). Course generator hooks **Col → Principled Base Color** so COLOR_0 is written.

If your export lacks colors:

- Connect Attribute / Color Attribute node to Base Color, or  
- Use Fast64 materials (L21), which are designed for this pipeline  

## Island exercise

Paint a darker strip across a ground plane as a path. Rebuild via inject + `gltf_to_t3d` or Fast64 export. View in [Asset lab](./checkpoint).

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[L21 — Fast64 & export](./l21-fast64-export).
