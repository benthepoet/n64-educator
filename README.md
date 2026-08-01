# N64 Educator

[![Docs](https://img.shields.io/badge/docs-GitHub%20Pages-blue)](https://benthepoet.github.io/n64-educator/)
[![License: MIT](https://img.shields.io/badge/code-MIT-green.svg)](LICENSE)
[![Docs license: CC BY 4.0](https://img.shields.io/badge/docs-CC%20BY%204.0-lightgrey.svg)](LICENSE-DOCS)

**Open-source curriculum** for making **3D games on the Nintendo 64** with
[libdragon](https://libdragon.dev/) (preview) and
[Tiny3D](https://github.com/HailToDodongo/tiny3d).

Start from a working toolchain and a “Hello ROM,” learn 3D from zero (no prior
Blender or graphics experience required), and finish a small third-person
collectathon: **Starshard Cove**.

> Unofficial community project. Not affiliated with Nintendo.

## What you will learn

- libdragon project layout, game loop, controllers, ROM filesystem  
- 3D literacy: vectors, matrices, cameras, meshes, lighting, vertex color  
- Tiny3D: first draws, depth, orbit camera, lights, model load  
- Blender + Fast64 (or course inject helper): props, paint, export, simple anim  
- Gameplay: third-person move, follow cam, collision, entities, state machines  
- Audio, HUD, juice, and a complete mini-game  

## Repository layout

```text
docs/                 # VitePress curriculum site
lessons/              # Buildable lesson ROMs (l01 … l34, labs, checkpoints)
common/               # Shared Makefiles + ng_math / ng_game helpers
capstone/             # Starshard Cove (final game)
assets-src/           # Masters + ATTRIBUTION.md
scripts/              # toolchain install, asset build, checks
design/               # Design notes
```

## Quick start

### 1. Toolchain (user-local, no root)

```bash
./scripts/install-toolchain.sh
source scripts/env.sh
```

See [docs/guide/setup.md](docs/guide/setup.md) for Docker / native alternatives.

### 2. Build a lesson

```bash
make l01
# → lessons/l01-hello-rom/l01_hello.z64
```

Open the ROM in [Ares](https://github.com/ares-emulator/ares) with **Homebrew mode** enabled.

### 3. Browse the curriculum

**Live site (GitHub Pages):**  
https://benthepoet.github.io/n64-educator/

Local preview:

```bash
npm install
npm run docs:dev
```

### 4. Capstone

```bash
make cove
# → capstone/starshard-cove/starshard_cove.z64
```

### Build everything

```bash
./scripts/build-all.sh
```

## Prerequisites

- Comfortable with C (structs, pointers, multi-file projects, Make)  
- Linux or macOS preferred; Windows via WSL2 or Docker  
- No prior 3D, Blender, or N64 experience required  

## Status

**Version:** see [`VERSION`](VERSION) (currently **1.1.3**).  
**Branch:** development happens on **`master`**.  
**Docs:** https://benthepoet.github.io/n64-educator/

Course Modules 0–5 are complete. Expect revisions as libdragon preview and
Tiny3D evolve — pins live in
[docs/reference/versions.md](docs/reference/versions.md).

History: **[CHANGELOG.md](CHANGELOG.md)** (Keep a Changelog + SemVer).

### GitHub Pages

The VitePress site deploys from **`master`** via `.github/workflows/docs.yml`.

One-time setup (repo admin):

1. **Settings → Pages → Build and deployment → Source:** *GitHub Actions*  
2. Push to `master` (or run the **Docs** workflow manually under Actions)

Site URL: `https://benthepoet.github.io/n64-educator/`

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) and
our [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

High-impact help: clearer noob comments, OS-specific setup fixes, better
troubleshooting, and CC0/CC-BY art upgrades.

## License

| Material | License |
|----------|---------|
| Source code, scripts, Makefiles, lesson C | [MIT](LICENSE) |
| Curriculum docs under `docs/` | [CC BY 4.0](LICENSE-DOCS) |
| Course-generated props (see attribution) | [CC0 1.0](assets-src/ATTRIBUTION.md) |
| Third-party bundled samples | See [assets-src/ATTRIBUTION.md](assets-src/ATTRIBUTION.md) and [NOTICE](NOTICE) |

Upstream tools (libdragon, Tiny3D, Fast64, Blender, Ares) remain under their
own licenses. You install them separately; this repo teaches you how.

## Credits

- [libdragon](https://github.com/DragonMinded/libdragon) contributors  
- [Tiny3D](https://github.com/HailToDodongo/tiny3d) — Max Bebök (HailToDodongo); sample models/textures (MIT)  
- [Quaternius](https://quaternius.com/) — Easy Enemy / snake character (CC0)  
- N64brew community  

Full asset inventory: **[assets-src/ATTRIBUTION.md](assets-src/ATTRIBUTION.md)** · **[NOTICE](NOTICE)**

## Trademark

Nintendo 64 and N64 are trademarks of Nintendo. This project is unofficial and
not endorsed by Nintendo.
