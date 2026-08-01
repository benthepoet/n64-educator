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

[Unreleased]: https://github.com/benthepoet/n64-educator/compare/v1.1.0...HEAD
[1.1.0]: https://github.com/benthepoet/n64-educator/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/benthepoet/n64-educator/releases/tag/v1.0.0
