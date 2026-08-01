# L23 — Level layout + vertex-color pass

::: tip Goal
Assemble a **single small island** (or modular pieces) and paint large-scale tints for paths and biomes.
:::

## Layout options

| Approach | Pros |
|----------|------|
| One grid/plane with height | Fast for course |
| Modular platforms snapped | Reuse pieces |
| Sculpt lightly then decimate | Organic; watch polycount |

Course `island` is a subdivided grid with a soft dome and painted path.

## Placement hygiene

- Keep gameplay space inside a clear boundary (fall-off for kill plane later)  
- Leave room for 8 starshard spots  
- Avoid microscopic gaps that break simple collision later  

## Collision proxies (preview)

Optional empties or simple cubes named `COL_*` for later gameplay — Module 4 can read a table even if auto-export isn’t ready. For now, notes in a text file are fine.

## Lesson code

Module 3 is Blender-first — there is no per-lesson C ROM for L17–L25.

Runnable code that loads the course meshes lives in the
[Module 3 asset lab checkpoint](./checkpoint) (`lessons/m3-asset-lab/`).

## Next

[L24 — Rigging](./l24-rigging).
