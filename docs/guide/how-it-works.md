# How this course works

## Lesson shape

Each lesson has:

1. **Concept** — mental model and diagrams
2. **N64 / Tiny3D notes** — what is special on this platform
3. **Hands-on** — code and/or Blender steps
4. **Build & run** — exact commands, what you should see
5. **Exercises** — optional stretch
6. **Next** — bridge to the following lesson

Canonical prose lives under `docs/`. Runnable projects live under `lessons/` (and later `capstone/`).

## Repository map

```text
lessons/l01-hello-rom/     # example lesson
  Makefile                 # sets ROMNAME, includes common/lesson.mk
  src/main.c
  README.md                # short pointer to the docs page
common/lesson.mk           # shared build rules
docs/guide/...             # what you are reading
assets-src/                # Blender, textures, audio masters
```

## Build pattern

From the repo root (with toolchain available):

```bash
make -C lessons/l01-hello-rom
```

Or with Docker CLI:

```bash
libdragon make -C lessons/l01-hello-rom
```

Output ROM: `lessons/l01-hello-rom/<romname>.z64`.

## Art and non-artists

Every art-facing lesson ships **canonical assets**. Completing the course never requires winning a modeling contest — replace assets when you want.

## Animation & vertex color

These are **core** topics (not optional extras):

- **Vertex color blending** — tint terrain/props with per-vertex color × texture (classic N64 look, cheap on TMEM).
- **Skeletal idle + walk** — simple armature, Tiny3D skinning limits explained, clips driven by move speed.

## Capstone

**Starshard Cove** reuses patterns from earlier lessons. Module 4–5 are aimed at that design on purpose — see the [one-page GDD](/guide/m5/starshard-cove).

## Getting help

- Re-read the lesson’s troubleshooting box
- Check [Glossary](/reference/glossary) and [Versions](/reference/versions)
- [N64brew Discord](https://discord.gg/WqFgNWf) for libdragon / Tiny3D community help

Next: [L01 — Hello ROM](/guide/m0/l01-hello-rom).
