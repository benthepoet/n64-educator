# Scale & conventions

Keep the whole course consistent so assets and code plug together.

## Units & axes

| Convention | Choice |
|------------|--------|
| World up | **+Y** |
| Typical forward | **−Z** or **+Z** — match Tiny3D examples; document in first 3D lesson and stick to it |
| Scale | **1.0 ≈ 1 meter** for human-scale props; player ~1.5–2.0 tall |
| Angles in course math notes | Degrees in prose; radians in C when APIs require |

## Player / camera (capstone)

- Follow camera sits behind and above the player
- Move is camera-relative on the XZ plane
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
