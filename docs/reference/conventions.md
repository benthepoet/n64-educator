# Scale & conventions

Keep the whole course consistent so assets and code plug together.

## Units & axes

| Convention | Choice |
|------------|--------|
| World up | **+Y** |
| Typical forward | **−Z** or **+Z** — match Tiny3D examples; document in first 3D lesson and stick to it |
| Gameplay scale | **1.0 ≈ 1 meter** for human-scale props; player ~1.5–2.0 tall |
| Course `t3dm` scales | Raw verts are large integers; ROMs apply e.g. island **~0.032**, snake **~0.02**, starshard **~0.02** so the world fits a ~12-unit island and a short follow cam |
| Snake model yaw | Course snake mesh faces **−Z**; pass **`-yaw`** into `t3d_mat4fp_from_srt_euler` so stick left/right match the model |
| Angles in course math notes | Degrees in prose; radians in C when APIs require |

## Display / VI

| Topic | Course convention |
|-------|-------------------|
| Resolution | `320×240`, `DEPTH_16_BPP` |
| VI filters | **`FILTERS_RESAMPLE` only** — avoid `FILTERS_RESAMPLE_ANTIALIAS` (can leave a 1px green/cyan flicker on the top edge) |
| Clear color | **Opaque** (`alpha 0xFF`) with Tiny3D `t3d_screen_clear_color` |

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

## Player / camera (Module 4–5 + capstone)

| Topic | Course convention |
|-------|-------------------|
| `camYaw` | Horizontal camera angle only (C-left/right). **Not** player facing |
| Move basis | Rotate stick by the **lagged camera** direction (eye→player on XZ), not by player yaw. Fallback to `camYaw` if eye is on top of the player |
| Player yaw | Model facing only — updates toward move direction; never drives the camera basis |
| Soft-follow | **Do not** lerp `camYaw` toward player yaw (they sit ~π apart → continuous spin) |
| Soft wall | **Axis-aligned box** clamp (e.g. ±5.5), not a radial cylinder (radial skates the rim) |
| Stick-up | Into the scene (away from camera), along look on XZ |

## Audio (Module 5 + capstone)

| Topic | Course convention |
|-------|-------------------|
| Opus `wav64` | Pipeline uses `--wav-compress 3` → **48 kHz** samples |
| Init | **`audio_init(48000, …)`** — lower rates assert on play (`frequency 48000 exceeds configured limit`) |
| Per frame | `mixer_try_play()` after (or with) the present |

## Textures

- Prefer **32×32** or **64×64** for props; avoid huge maps
- Power-of-two dimensions
- Vertex color for large-scale tinting instead of unique giant textures

## Vertex color (Fast64 / Tiny3D)

- Color attribute / layer names: **`Col`** and **`Alpha`** (Fast64 convention)
- Course default material path: **texture modulated by vertex color** for terrain
- Always ship a pre-exported glTF that already has correct layers

## Animation

- Clips required for capstone: **`idle`**, **`walk`** (course names: `Snake_Idle` / `Snake_Walk`)
- Looping, on-the-spot cycles
- Low bone count; respect Tiny3D 1-bone-per-vertex weighting
- Course ships a reference armature under `assets-src/blender/`

## Code style (light)

- C11 as supported by libdragon’s GCC
- Prefer clear names over clever macros
- Match libdragon / Tiny3D example style so Discord help is easy
