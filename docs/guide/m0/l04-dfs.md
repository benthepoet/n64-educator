# L04 — Assets on ROM (DFS)

::: tip Goal
Understand that game assets live **inside the ROM**, convert a PNG to a libdragon sprite, pack it with the **Dragon File System (DFS)**, and draw it with RDPQ.
:::

## What you will see

A bobbing **star** sprite and text explaining the pipeline:

```text
PNG → mksprite → DFS → sprite_load("rom:/…")
```

## Why not `fopen("star.png")` on your PC?

The finished game is a single **`.z64`** image. On real hardware there is no access to your laptop’s files. Assets must be:

1. Converted to console-friendly formats at **build time**
2. **Embedded** in the ROM
3. Loaded at runtime via paths like `rom:/star.sprite`

That embedded filesystem is **DFS** (Dragon File System).

## Build pipeline

```text
assets/star.png
       │
       │  mksprite  (host tool)
       ▼
filesystem/star.sprite
       │
       │  mkdfs + n64tool  (link into .z64)
       ▼
rom:/star.sprite   ←  sprite_load() at runtime
```

Course Makefiles do this automatically when `assets/*.png` exists (`common/lesson.mk`).

### Lesson Makefile note

```make
MKSPRITE_FLAGS := -f RGBA16
include ../../common/lesson.mk
```

`RGBA16` is a simple full-color format for teaching. Later you will meet CI4/CI8 palettes and TMEM limits (L05 + graphics modules).

## Runtime code

```c
dfs_init(DFS_DEFAULT_LOCATION);

sprite_t *star = sprite_load("rom:/star.sprite");

/* in render: */
rdpq_set_mode_standard();
rdpq_mode_alphacompare(1);   /* skip fully transparent texels */
rdpq_sprite_blit(star, x, y, NULL);
```

| Call | Role |
|------|------|
| `dfs_init` | Mount the ROM filesystem (once at startup) |
| `sprite_load("rom:/…")` | Load a packed sprite by **ROM path** |
| `rdpq_sprite_blit` | Draw it with the RDP |

::: warning Path prefix
Always use the `rom:/` prefix for DFS assets. A bare `star.sprite` or a host path will not work on console.
:::

## Build & run

```bash
source scripts/env.sh
make -C lessons/l04-dfs
# → lessons/l04-dfs/l04_dfs.z64
```

`make clean` in the lesson folder also removes generated `filesystem/`.

## Exercises

1. Replace `assets/star.png` with your own 32×32 or 64×64 PNG; rebuild.
2. Draw the sprite in two places (two blits).
3. (Stretch) Add a second PNG and load both.

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Red screen / load failed | Asset not packed — check `filesystem/star.sprite` after build |
| Invisible sprite | Wrong path; or fully transparent PNG; enable alphacompare carefully |
| Build skips sprite | Put PNG under `assets/` with `.png` extension |



## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l04-dfs/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l04-dfs/Makefile) · [`lessons/l04-dfs/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l04-dfs/src/main.c)

::: details `lessons/l04-dfs/Makefile`
<<< ../../../lessons/l04-dfs/Makefile{make}
:::

::: details `lessons/l04-dfs/src/main.c`
<<< ../../../lessons/l04-dfs/src/main.c{c}
:::

## What you learned

- Assets are **build-time** converted and **ROM-embedded**
- DFS + `rom:/` paths
- `mksprite` + `sprite_load` + `rdpq_sprite_blit`

## Next

[L05 — N64 hardware tour](/guide/m0/l05-hardware) names the chips you have been using, then the [Module 0 checkpoint](/guide/m0/checkpoint) proves L01–L04 together.
