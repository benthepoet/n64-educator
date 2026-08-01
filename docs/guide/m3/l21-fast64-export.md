# L21 — Fast64 materials & glTF export

::: tip Goal
Export a model Tiny3D accepts: either **Fast64** (best) or the course **f3d inject** helper (good enough for class props).
:::

## Path A — Fast64 (recommended for real games)

1. Install [Fast64](https://github.com/Fast-64/fast64) into Blender.  
2. Create **F3D materials** (not only Principled).  
3. Use a **vertex-colored texture** preset when blending texture × `Col`.  
4. Export glTF Binary:  
   - **Include → Custom Properties** enabled  
   - Selected objects if needed  
5. Convert:

```bash
source scripts/env.sh
gltf_to_t3d myprop.glb filesystem/myprop.t3dm
mkasset -c 2 -o filesystem filesystem/myprop.t3dm
```

Place textures (PNG) beside the export; importers/tools emit sprites.

### Export footguns

| Symptom | Fix |
|---------|-----|
| `Material has no fast64 data` | Custom Properties off, or not F3D material |
| Missing textures | PNG path wrong; re-export; check sprite names |
| Inside-out mesh | Recalculate normals outside; winding |
| Huge/tiny | Apply scale; match course units |

## Path B — Course inject (no Fast64)

For placeholders and class machines without the addon:

```bash
python3 scripts/gltf_inject_f3d.py raw.glb fixed.glb
gltf_to_t3d fixed.glb out.t3dm
```

`common/lesson.mk` runs this inject automatically when converting `assets/*.glb`.

::: warning Inject is a teaching bridge
It attaches a generic shade/lighting material. It will not replace learning Fast64 combiners for production look.
:::

## Lesson Makefile integration

Drop files in a lesson’s `assets/`:

- `*.glb` → inject → `.t3dm`  
- `*.png` → `.sprite`  
- prebuilt `*.t3dm` / `*.sdata` copied through  

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[L22 — Player & platform](./l22-player-platform).
