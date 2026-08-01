# L19 — UVs & textures (beginner)

::: tip Goal
Understand **UVs** as “gift wrap coordinates,” create a tiny texture, and respect N64 size limits.
:::

## UVs in one metaphor

The mesh is a cardboard animal. **UV unwrap** flattens its skin onto a 2D poster. The texture image is paint on that poster.

- **U, V** = axes of the poster (like X, Y for images)  
- Overlapping UVs share paint (fine for tiling)  
- Stretching UVs stretches the art  

## Minimal Blender path

1. Select mesh → Edit Mode.  
2. Select all (`A`).  
3. UV → Unwrap (or Smart UV Project for a first pass).  
4. Open UV Editor to see the island.  

## Texture rules for this course

| Do | Don’t |
|----|--------|
| 32×32 or 64×64 to start | 2K hero textures |
| Power-of-two sizes | Random 137×91 |
| Few unique textures | Unique 256×256 per ground tile |
| Prefer PNG masters | Uncompressed huge TIFFs in ROM |

Recall **TMEM ~4 KiB** (L05). Vertex color (L20) carries large-scale variation.

## Assign a test image

1. Create a 64×64 PNG (checker or solid).  
2. Material → Base Color → Image Texture.  
3. Pack or keep the PNG next to the blend for export.  

## Exercises

1. Smart-UV a cube and put a checker on it.  
2. Resize an image to 32×32 and reassign.  

## Next

[L20 — Vertex paint](./l20-vertex-paint).
