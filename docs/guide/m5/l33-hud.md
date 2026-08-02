# L33 — HUD & 2D over 3D

::: tip Goal
Draw a readable **HUD** after the 3D pass: shard count, timer, pause/title copy.
:::

## In plain English

Order matters:

```text
1. Attach color + depth
2. Draw 3D (Tiny3D)
3. rdpq_set_mode_standard + text  ← HUD
4. detach_show
5. mixer_try_play
```

If you draw text *before* clearing for 3D, it vanishes.

## What you will see

```bash
make -C lessons/l33-hud
```

Toy Cove with audio + always-visible **Shards n/3** and **Time** in play mode.

Same rules as L32/L34: **`audio_init(48000)`**, island/snake/shard **scales**,
**push/pop**, **`t3d_skeleton_use`**, eye-relative move + `camYaw`, box wall,
**`FILTERS_RESAMPLE`**.

## HUD tips

- Put critical info top corners (safe from camera chaos)  
- Pause should freeze gameplay and show big centered text  
- Don’t overdraw — N64 text is precious for readability  
- Draw HUD **after** 3D (`rdpq_set_mode_standard` then text)  



## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l33-hud/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l33-hud/Makefile) · [`lessons/l33-hud/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l33-hud/src/main.c)

::: details `lessons/l33-hud/Makefile`
<<< ../../../lessons/l33-hud/Makefile{make}
:::

::: details `lessons/l33-hud/src/main.c`
<<< ../../../lessons/l33-hud/src/main.c{c}
:::

## Next

[L34 — Juice](./l34-juice).
