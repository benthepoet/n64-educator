# L17 — Blender survival kit

::: tip Goal
Install Blender, move around a scene without panic, and learn the **few** modes and transforms you need for this course.
:::

## Install

1. Download Blender from [blender.org](https://www.blender.org/) (4.2 LTS or newer).  
2. Optional but recommended: [Fast64](https://github.com/Fast-64/fast64) addon (L21).  
3. Course batch scripts also work headless: `blender -b -P …`

## The only controls that matter at first

| Action | Default |
|--------|---------|
| Orbit view | Middle-mouse drag (or Alt+LMB depending on keymap) |
| Pan | Shift + middle-mouse |
| Zoom | Scroll wheel |
| Select | Left-click |
| Move / Rotate / Scale | **G** / **R** / **S** |
| Confirm / cancel | Enter/LMB / Esc/RMB |
| Undo | Ctrl+Z |

::: tip Prefer the “Industry Compatible” keymap?
Fine — but then rewrite the hotkeys above in your notes. Consistency beats purity.
:::

## Object mode vs Edit mode

| Mode | For |
|------|-----|
| **Object Mode** | Whole objects: place the crate in the level |
| **Edit Mode** (Tab) | Vertices/edges/faces: reshape the mesh |

If G/R/S does nothing useful, check you’re in the mode you think you are.

## Course scale

From [conventions](/reference/conventions):

- Aim for player height ~**1.5–2.0** Blender units  
- Keep props in the same scale family as Module 2 sample models  
- **Always Apply Scale** (`Ctrl+A` → Scale) before export if you resized in Object mode  

Unapplied scale is a classic “why is my model huge/tiny/lighting wrong?” bug.

## Save habits

- Save `.blend` under `assets-src/blender/yourname/`  
- Export `.glb` copies under `assets-src/export/`  
- Never only have a `.glb` — keep the `.blend` source  

## Exercises

1. Add a cube, move it, rotate it, scale it, undo.  
2. Tab to Edit Mode, select a face, extrude (**E**).  
3. File → Save your playground blend.

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[L18 — Model a starshard](./l18-starshard).
