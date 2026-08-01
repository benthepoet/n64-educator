# Scale & conventions

Keep the whole course consistent so assets and code plug together.

## Units & axes

| Convention | Choice |
|------------|--------|
| World up | **+Y** |
| Typical forward | **−Z** or **+Z** — match Tiny3D examples; document in first 3D lesson and stick to it |
| Gameplay scale | **1.0 ≈ 1 meter** for human-scale props; player ~1.5–2.0 tall |
| Course `t3dm` scales | Raw verts are large integers; ROMs apply e.g. island **~0.032**, snake **~0.02**, starshard **~0.02** so the world fits a ~12-unit island and a short follow cam |
| Angles in course math notes | Degrees in prose; radians in C when APIs require |

## Tiny3D matrix stack

After `t3d_viewport_attach`, the stack holds the **camera**. Per object:

```text
t3d_matrix_push(modelMat);
t3d_model_draw(...) / vert_load(...);
t3d_matrix_pop(1);
```

Avoid `t3d_matrix_set` in that slot unless you fully understand the stack — a common
failure mode is clear color + HUD text with no 3D.

Buffered skeletons: `t3d_skeleton_use(&skel)` before `t3d_model_draw_skinned`.

## Player / camera (capstone)

- Follow camera sits behind and above the player
- Move is camera-relative on the XZ plane (stick-up = into the scene)
- Kill plane well below the island

## Textures

- Prefer **32×32** or **64×64** for props; avoid huge maps
- Power-of-two dimensions
- Vertex color for large-scale tinting instead of unique giant textures

## Vertex color (Fast64 / Tiny3D)

- Color attribute / layer names: **`Col`** and **`Alpha`** (Fast64 convention)
- Course default material path: **texture modulated by vertex color** for terrain
- Always ship a pre-exported glTF that already has correct layers

## Animation

- Clips required for capstone: **`idle`**, **`walk`**
- Looping, on-the-spot cycles
- Low bone count; respect Tiny3D 1-bone-per-vertex weighting
- Course ships a reference armature under `assets-src/blender/`

## Code style (light)

- C11 as supported by libdragon’s GCC
- Prefer clear names over clever macros
- Match libdragon / Tiny3D example style so Discord help is easy
