# L18 — Model a collectible (starshard)

::: tip Goal
Build a tiny **low-poly collectible** from primitives. Readable silhouette > fancy topology.
:::

## Target

Something that reads as a **shiny pickup** from behind a follow camera: crystal, star, gem — your call. Course placeholder is a stretched icosphere (`starshard`).

## Steps (minimum path)

1. Delete default cube if you want a clean scene.  
2. Add → Mesh → Ico Sphere (subdivisions **1**).  
3. Scale Z up a bit (crystal).  
4. Object → Apply → Scale.  
5. Optional: shade smooth.  
6. Save `starshard.blend`.

Keep **triangle count low** (tens, not thousands).

## Course generator (optional)

```bash
./scripts/build-course-assets.sh
# writes assets-src/blender/generated/starshard.blend + .glb
```

Use this if you want a known-good mesh while learning UI.

## Check

- Object origin near center/bottom (easier to place in game)  
- Not microscopic / not planet-sized vs player  

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[L19 — UVs & textures](./l19-uvs) — or skip to vertex paint if you only want colors first (L20). For N64, **vertex paint alone is valid**.
