# Asset attribution

This file is the **canonical list** of third-party and course-generated assets
shipped with N64 Educator. Keep it accurate when you add or replace content.

Course **code** is MIT (`LICENSE`). Curriculum **docs** are CC BY 4.0
(`LICENSE-DOCS`). This file covers **art, audio, and sample binaries** under
`assets-src/`, `lessons/*/assets/`, and `capstone/*/assets/`.

---

## Course-generated props (CC0 1.0)

Produced by `scripts/blender_make_course_props.py` (and related pipeline scripts).
Masters under `assets-src/blender/generated/` and `assets-src/export/`; converted
copies appear as `*.t3dm` in lesson/capstone `assets/` folders.

| Asset name | Role |
|------------|------|
| `starshard` | Crystal collectible |
| `platform` | Simple platform mesh |
| `island` | Vertex-painted ground grid |
| `player_static` | Blockout character (no skeletal animation) |

**License:** [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)  
Public-domain dedication. Attribution is not required; credit is appreciated.

### Course-generated 2D

| File | Role |
|------|------|
| `star.png` (e.g. `lessons/l04-dfs/assets/`, Module 0 checkpoint) | Tiny teaching sprite for DFS / sprite blit lessons |

**License:** CC0 1.0 (same as course-generated props).

---

## Reference animated character (`player_anim` / snake)

**Origin chain:**

1. **Quaternius** — *Easy Enemy* pack  
   https://quaternius.com/packs/easyenemy.html  
   **License:** [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/)

2. Redistributed in **Tiny3D** as `examples/08_animation` (`snake.glb`, etc.)  
   https://github.com/HailToDodongo/tiny3d  

3. This course renames/ships it as **`player_anim`** (`.glb`, `.t3dm`, and
   `player_anim.N.sdata` animation streams) so Modules 3–5 can teach skinning
   and idle/walk blend without blocking on rigging.

**Clip names** such as `Snake_Idle` / `Snake_Walk` come from the upstream glTF.

**Licenses to honor:** Quaternius **CC0** (character art); Tiny3D **MIT** for
the packaging/tooling lineage (see notice below).

---

## Tiny3D example models & textures (MIT)

These teaching samples are taken from **Tiny3D** examples (HailToDodongo /
Max Bebök). Tiny3D is **MIT** — retain the copyright notice when redistributing
substantial portions (see `NOTICE` and the notice block below).

| Course path (typical) | Upstream | Notes |
|-----------------------|----------|--------|
| `lessons/l15-load-model/assets/model.glb` (+ `model.t3dm`) | Tiny3D `examples/01_model` | Spinning lit sample mesh |
| `stone01.i8.png`, `unit1m.i8.png` | Same `01_model` assets | Textures for L15 model |
| `map.glb` / `map.t3dm` | Tiny3D `examples/08_animation` | Ground map used with the snake |
| `sand12.ci4.png`, `stone.ci4.png`, `shadow.i8.png` | Tiny3D `08_animation` (and related) | Map / shadow textures |
| `player_anim*` | See previous section | Character + `.sdata` via Tiny3D packaging |

**Tiny3D project:** https://github.com/HailToDodongo/tiny3d  

```
Copyright (c) 2023 Max Bebök

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

Embedded art inside those examples may also carry **Quaternius CC0** (snake) or
other notices from the Tiny3D tree; keep this file when forking.

---

## Audio (teaching samples)

Bundled under `assets-src/audio/` and copied into lesson/capstone `assets/`:

| File | Origin (approx.) | Notes |
|------|------------------|--------|
| `collect.wav` | libdragon examples (`brew-volley` / related) | Short SFX |
| `ui.wav` | libdragon `mixertest` lineage (`cannon.wav` renamed for course UX) | Short SFX |
| `win.wav` | libdragon `brew-volley` examples | Short SFX |
| `music.xm` | libdragon `audioplayer` examples (`ToysXM-8bit.xm`) | Tracker music |

**libdragon** is released under the **Unlicense**:  
https://github.com/DragonMinded/libdragon  

The brew-volley example notes it is a modified jam entry released with the
author’s permission as a libdragon example (see upstream
`examples/brew-volley/README.md`). Individual sample authors may be credited
inside those trees. When redistributing **this** repository, keep this
ATTRIBUTION file.

For a commercial-style game of your own, prefer replacing example SFX/music
with tracks you fully control or that have licenses you accept.

---

## Software you must install separately

Not redistributed as complete SDKs in this tree (you install via scripts/docs):

| Project | License (upstream) | URL |
|---------|-------------------|-----|
| libdragon | Unlicense | https://github.com/DragonMinded/libdragon |
| Tiny3D | MIT | https://github.com/HailToDodongo/tiny3d |
| Fast64 | See repo | https://github.com/Fast-64/fast64 |
| Blender | GPL | https://www.blender.org/ |
| Ares emulator | See repo | https://github.com/ares-emulator/ares |

---

## Credits (quick list)

- **Quaternius** — Easy Enemy / snake character (CC0)  
- **Max Bebök (HailToDodongo) and Tiny3D contributors** — sample models, textures, toolchain (MIT)  
- **libdragon / N64brew community** — engine examples and audio samples (Unlicense / as noted upstream)  
- **N64 Educator contributors** — course-generated props, curriculum, packaging  

---

## Nintendo

Nintendo 64, N64, and related marks are trademarks of Nintendo.  
This educational project is unofficial and not endorsed by Nintendo.

No Nintendo proprietary ROMs, SDKs, or game assets are included.
