# Asset attribution

This file lists third-party and generated assets shipped with N64 Educator for
teaching. Keep this file accurate when you add or replace content.

## Course-generated props (CC0 1.0)

Produced by `scripts/blender_make_course_props.py` and related pipeline scripts:

- `starshard` (crystal collectible)
- `platform`
- `island` (vertex-painted ground)
- `player_static` (blockout character)

**License:** [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)  
You may use these without attribution, though credit is appreciated.

## Reference animated character (`player_anim`)

Derived from Tiny3D example assets (`examples/08_animation`), based on:

- **Quaternius** — Easy Enemy pack  
  https://quaternius.com/packs/easyenemy.html  
  **License:** CC0 1.0  

Shipped as a known-good skinned model with `Snake_Idle` / `Snake_Walk` clips so
Module 3–5 learners can study animation without blocking on rigging skill.

Also see Tiny3D’s own credits: https://github.com/HailToDodongo/tiny3d

## Audio (teaching samples)

Bundled under `assets-src/audio/` and copied into lesson/capstone `assets/`:

| File | Origin (approx.) | Notes |
|------|------------------|--------|
| `collect.wav` | libdragon examples (`brew-volley` / related packs) | Short SFX |
| `ui.wav` | libdragon `mixertest` lineage (`cannon.wav` renamed for course UX) | Short SFX |
| `win.wav` | libdragon `brew-volley` examples | Short SFX |
| `music.xm` | libdragon `audioplayer` examples (`ToysXM-8bit.xm`) | Tracker music |

**libdragon** itself is released under the **Unlicense**:  
https://github.com/DragonMinded/libdragon  

Individual example asset authors may be credited inside those upstream trees
(e.g. music courtesy notes in libdragon demos). When redistributing **this**
repository, keep this ATTRIBUTION file.

For a public commercial-style game of your own, prefer replacing example SFX/music
with tracks you fully control or that have clear licenses you accept.

## Map / texture scraps from Tiny3D demos

Some lessons may include small textures (`sand12.ci4.png`, `stone.ci4.png`,
`shadow.i8.png`) and `map` fragments copied from Tiny3D examples for the asset
lab. Those remain under Tiny3D’s MIT project licensing and any embedded art
notices from that repo.

## Software you must install separately

Not redistributed as complete SDKs in this tree (you install via scripts/docs):

| Project | License (upstream) | URL |
|---------|-------------------|-----|
| libdragon | Unlicense | https://github.com/DragonMinded/libdragon |
| Tiny3D | MIT | https://github.com/HailToDodongo/tiny3d |
| Fast64 | See repo | https://github.com/Fast-64/fast64 |
| Blender | GPL | https://www.blender.org/ |
| Ares emulator | See repo | https://github.com/ares-emulator/ares |

## Nintendo

Nintendo 64, N64, and related marks are trademarks of Nintendo.  
This educational project is unofficial and not endorsed by Nintendo.
