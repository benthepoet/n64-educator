# Changelog

All notable changes to **N64 Educator** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## How we version

| Kind | When |
|------|------|
| **MAJOR** (`X.0.0`) | Incompatible curriculum restructuring, or learner ROMs/docs that break older lesson numbering without a migration path |
| **MINOR** (`x.Y.0`) | New lessons, modules, or substantial content; new optional tools |
| **PATCH** (`x.y.Z`) | Fixes, clearer comments, typos, small doc polish, bugfixes that do not change the teaching path |

Toolchain pins (libdragon / Tiny3D commits) are recorded in `docs/reference/versions.md`.
Bump the course version when pins change in a way that affects learners.

**Every commit that lands on `master` (and is pushed) must bump `VERSION` /
`package.json` and move the change into a dated `CHANGELOG.md` section.** Do not
leave user-visible work only under `[Unreleased]` on `master`.

---

## [Unreleased]

### Added

### Changed

### Fixed

### Removed

---

## [1.1.9] — 2026-08-02

### Changed

- **Docs ↔ code audit:** conventions, Module 4–5 guides, and source file headers
  now match the fixed ROMs end-to-end:
  - free `camYaw` + move from **lagged eye→player** (not player yaw; no soft-follow)
  - **box** soft-walls (not radial)
  - snake model **`-yaw`**
  - **`FILTERS_RESAMPLE`** + opaque clear (no VI AA edge flicker)
  - **`audio_init(48000)`** for Opus `wav64`
  - course scales (island ~0.032 / snake ~0.02; L27 sample map still different)
  - L31 title always draws the scene; toy-cove doc path fix in source
- Capstone comments corrected (box wall, eye-relative move, far plane 200)

---

## [1.1.8] — 2026-08-02

### Fixed

- **Camera spin / stick feedback:** decouple `camYaw` from player facing; move
  relative to the lagged camera (eye→player), not `yaw + orbit` — L28–L31, L33,
  L34, Toy Cove, capstone
- **Remove soft-follow of camYaw toward player yaw** (angles ~π apart → continuous
  spin when running “forward”)
- **Box soft-walls** instead of radial clamp (radial skated along the rim) on
  L30–L31 and siblings
- **Snake facing:** model yaw is `-yaw` (mesh faces −Z) so left/right match stick
- **L31 title scene:** always draw island, shards, and idle player; assert DFS
  model loads; clamp dt; title orbit showcase

### Changed

- **Docs:** L28 follow-cam and L30 entities notes match the camera/move basis

---

## [1.1.7] — 2026-08-02

### Fixed

- **VI edge artifact:** use `FILTERS_RESAMPLE` (not `RESAMPLE_ANTIALIAS`) and
  opaque clear (`alpha 0xFF`) in 3D lessons — avoids a 1px flickering green/cyan
  line at the top of the framebuffer
- **L26:** smaller `player_static` scale (`0.04`) so the blockout character fits
  the island better
- **L28:** `assertf` if island/player fail to load from DFS (DeepSeek) so a
  missing asset fails loudly instead of a blank scene

---

## [1.1.6] — 2026-08-01

### Changed

- Curriculum docs re-audited against ROM code: matrix **push/pop** (not
  `matrix_set` over the camera), course **t3dm scales**, stick-up convention,
  skeleton_use, and 48 kHz audio notes on L12–L16, L26, L28–L31, L34, M3
  checkpoint, Module 4 overview, and reference conventions

---

## [1.1.5] — 2026-08-01

### Fixed

- More lessons use **`t3d_matrix_push` / `pop`** instead of `t3d_matrix_set` so
  model matrices compose with the camera stack (blank mesh / clear-color-only):
  L12–L14, L26, L30, m3-asset-lab, and Starshard Cove player draw

---

## [1.1.4] — 2026-08-01

### Fixed

- **Audio:** `audio_init(48000)` everywhere Opus `wav64` is used (was 44100 →
  assert `frequency 48000 exceeds configured limit` on SFX play)
- **Course island scale:** `island.t3dm` verts are ~±384; world scale **0.032**
  so gameplay cameras (~12u) can see the mesh (L26, L28–L34, Toy Cove, capstone)
- **Snake / player_anim scale** on island lessons: **0.02** (~1.6u tall), not
  Tiny3D map scale 0.125
- **Starshard scale** ~**0.02** so collectibles match the scaled island
- Skinned draw: `t3d_skeleton_use` + matrix push/pop; shard draws use push/pop
- L32 docs: sample-rate vs `audio_init` warning
- Sync `player_anim.t3dm` (with `rom:/` sdata paths) across anim lessons

---

## [1.1.3] — 2026-08-01

### Changed

- Curriculum docs aligned with recent Tiny3D bugfixes: L11/L12/L14/L16
  (combiner + manual draw path), L25 (`t3d_skeleton_use`), L26 (stick-up /
  camera-relative math), L27 (skeleton_use, sdata, sample map wording)

---

## [1.1.2] — 2026-08-01

### Fixed

- Camera-relative stick **up/down was inverted** (stick-up moved toward the
  camera). Forward is now along the camera look direction on XZ in L26–L31,
  L33–L34, Toy Cove, and Starshard Cove

---

## [1.1.1] — 2026-08-01

### Fixed

- **L15** model draw: recorded `t3d_model_draw` must use `t3d_matrix_push` /
  recorded `t3d_matrix_pop` (Tiny3D `01_model` pattern); `matrix_set` alone left
  a blank mesh on a valid clear color
- **L11–L14, L16** manual Tiny3D geometry: set `rdpq_mode_combiner(RDPQ_COMBINER_SHADE)`
  so vertex colors are not black
- **L16** terrain draw path (push/load/pop, int16 positions) and remove a stray
  `\\n` line that broke compilation in several lesson sources
- **L27** skinned character: bake animation streams as `rom:/player_anim.N.sdata`,
  call `t3d_skeleton_use` before `t3d_model_draw_skinned` for buffered skeletons,
  and align map/player scale and camera with Tiny3D’s known-good `08_animation` sample
- Same **`t3d_skeleton_use`** fix for L28–L31, L33–L34, m3-asset-lab, m4-toy-cove,
  and Starshard Cove
- **`common/lesson.mk`**: document that `gltf_to_t3d` output must live under
  `filesystem/` so animation `.sdata` paths get the `rom:/` prefix; prefer glb
  conversion over a same-named prebuilt `.t3dm`

### Changed

- **Attribution:** expand `assets-src/ATTRIBUTION.md` and `NOTICE` with an
  inventory of course CC0 props, Quaternius snake / `player_anim`, Tiny3D L15
  model + map textures (MIT / Max Bebök notice), and libdragon audio samples
- Docs license page and README credits aligned with that inventory
- L27 teaching assets: sample map + correctly converted snake character (still
  Quaternius CC0 via Tiny3D examples)

---

## [1.1.0] — 2026-08-01

### Added

- **In-browser lesson source:** VitePress imports real `Makefile` / `src/main.c`
  (and shared `common/` library files) into the curriculum site via `<<<` snippets
  so docs stay in sync with the repo
- Full source sections on all code lessons (L01–L04, L06–L16, L26–L34), Module 0 /
  Module 3 checkpoints, Toy Cove, and Starshard Cove
- Shared library source on [How this course works](docs/guide/how-it-works.md)
  (`common/lesson.mk`, `ng_math`, `ng_game`)
- Pointers from Blender-only Module 3 lessons (L17–L25), L05 hardware, and L36
  shipping to the runnable projects that hold the code
- GitHub Pages hosting for the VitePress curriculum
  (`https://benthepoet.github.io/n64-educator/`)
- Docs deploy workflow (build + `actions/deploy-pages`)

### Changed

- VitePress `base` is `/n64-educator/` in CI (local dev still uses `/`)
- Docs Vite config allows reading lesson sources outside `docs/` for snippets
- Maintainer rule: ship to `master` with matching `VERSION`, `package.json`, and
  dated changelog entry (no silent docs-only pushes)

---

## [1.0.0] — 2026-08-01

First public open-source release of the full curriculum.

### Added

- Modules **0–5** curriculum (docs site via VitePress)
- Lesson ROMs from Hello ROM through polish, plus checkpoints:
  - Module 0 foundations + checkpoint
  - Module 1 3D literacy (`ng_math`)
  - Module 2 Tiny3D first light
  - Module 3 Blender pipeline + asset lab
  - Module 4 gameplay systems + Toy Cove
  - Module 5 audio / HUD / juice
- Capstone **Starshard Cove** (`capstone/starshard-cove`)
- Shared build fragment `common/lesson.mk` (sprites, glTF→t3dm, audio, sdata)
- Toolchain install scripts (`scripts/install-toolchain.sh`, `scripts/env.sh`)
- Asset pipeline helpers (`build-course-assets.sh`, `gltf_inject_f3d.py`, Blender prop generator)
- Open-source meta: MIT code, CC BY 4.0 docs, NOTICE, CONTRIBUTING, Code of Conduct, SECURITY
- Third-party attribution under `assets-src/ATTRIBUTION.md`

### Notes

- Default git branch: **`master`**
- Requires libdragon **preview** + Tiny3D (see pinned versions doc)

[Unreleased]: https://github.com/benthepoet/n64-educator/compare/v1.1.7...HEAD
[1.1.7]: https://github.com/benthepoet/n64-educator/compare/v1.1.6...v1.1.7
[1.1.6]: https://github.com/benthepoet/n64-educator/compare/v1.1.5...v1.1.6
[1.1.5]: https://github.com/benthepoet/n64-educator/compare/v1.1.4...v1.1.5
[1.1.4]: https://github.com/benthepoet/n64-educator/compare/v1.1.3...v1.1.4
[1.1.3]: https://github.com/benthepoet/n64-educator/compare/v1.1.2...v1.1.3
[1.1.2]: https://github.com/benthepoet/n64-educator/compare/v1.1.1...v1.1.2
[1.1.1]: https://github.com/benthepoet/n64-educator/compare/v1.1.0...v1.1.1
[1.1.0]: https://github.com/benthepoet/n64-educator/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/benthepoet/n64-educator/releases/tag/v1.0.0
