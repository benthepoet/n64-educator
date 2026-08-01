# Setup & toolchain

This page gets you from zero to a built ROM. The **recommended path for this course** is a **user-local install** under `~/n64-dev` (no root, works on Arch/CachyOS/etc.). Docker and system-wide packages remain valid alternatives.

::: warning Preview branch required
3D support (and Tiny3D) needs **libdragon `preview`**, not only the stable `trunk`. Pins: [Pinned versions](/reference/versions).
:::

## What you need

| Tool | Role |
|------|------|
| MIPS GCC toolchain + libdragon **preview** | Compile C → N64 ROM |
| Tiny3D | 3D pipeline (from Module 2; installed by our script) |
| Ares emulator | Run ROMs; enable **Homebrew mode** |
| Git, curl, make, a C/C++ host compiler | Build tools and libs |
| (Module 3+) Blender 4.x + Fast64 | Art pipeline |

---

## Option A — Course installer (recommended)

From the **repository root**:

```bash
./scripts/install-toolchain.sh
source scripts/env.sh
make l01
```

That will:

1. Download the official **gcc-toolchain-mips64** `.deb` and extract it to `~/n64-dev/toolchain` (**no root**).
2. Clone and build **libdragon `preview`**, install headers/libs/tools into that prefix.
3. Clone and build **Tiny3D**, install `libt3d` + `gltf_to_t3d`.

Activate the environment in every new shell:

```bash
source scripts/env.sh
```

| Variable | Default |
|----------|---------|
| `N64_DEV` | `$HOME/n64-dev` |
| `N64_INST` | `$N64_DEV/toolchain` |
| `T3D_INST` | `$N64_DEV/tiny3d` |

Override install location:

```bash
N64_DEV=/data/n64-dev ./scripts/install-toolchain.sh
```

### Layout after install

```text
~/n64-dev/
  toolchain/          # N64_INST — gcc, n64.mk, libdragon, Tiny3D
  libdragon/          # source checkout (preview)
  tiny3d/             # source checkout
  downloads/          # cached .deb
  VERSIONS.txt        # optional local pin note
```

### Build lessons

```bash
source scripts/env.sh
make l01                          # lessons/l01-hello-rom/l01_hello.z64
make l02
./scripts/build-all.sh            # every lesson Makefile
```

---

## Option B — Docker

The community [libdragon-docker](https://github.com/anacierdem/libdragon-docker) wrapper runs the toolchain in a container:

```bash
npm install -g libdragon
libdragon init
libdragon make -C lessons/l01-hello-rom
```

Ensure the container uses **libdragon preview** and a Tiny3D install matching [Pinned versions](/reference/versions). Prefer Option A if Docker is not already part of your workflow.

---

## Option C — Official packages + manual libdragon

1. Install the toolchain from [libdragon releases](https://github.com/DragonMinded/libdragon/releases/tag/toolchain-continuous-prerelease) (`.deb` / `.rpm` with root, or extract like our script).
2. `export N64_INST=/opt/libdragon` (or your prefix).
3. Clone libdragon `preview`, `make && make install`, then tools.
4. Clone Tiny3D, `./build.sh`.

Details: [Installing libdragon](https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon).

---

## Emulator — Ares

1. Install [Ares](https://github.com/ares-emulator/ares).
2. Load `lessons/l01-hello-rom/l01_hello.z64`.
3. Enable **Homebrew mode**.

Older emulators often fail on modern libdragon ROMs.

---

## Verify your install

You are ready when:

1. `source scripts/env.sh && make l01` succeeds.
2. Ares shows L01 text on a dark blue background.
3. `ls $N64_INST/include/t3d.mk` exists (Tiny3D ready for Module 2).

Next: [How this course works](/guide/how-it-works), then [L01 — Hello ROM](/guide/m0/l01-hello-rom).

## Troubleshooting

| Symptom | Things to check |
|---------|-----------------|
| `N64_INST is not set` | `source scripts/env.sh` |
| `n64.mk does not exist` | Re-run `./scripts/install-toolchain.sh` |
| `n64tool: Need output flag…` | ROM title quoting in Makefile (use course `common/lesson.mk`) |
| ROM black screen in old emulator | Use Ares + Homebrew mode |
| Built against wrong branch | Rebuild **preview**; check [versions](/reference/versions) |
| `ar: command not found` when installing | Install `binutils` (provides `ar`) for `.deb` extract |
| Host compiler missing for tools | Install `gcc` / `g++` on the host |
