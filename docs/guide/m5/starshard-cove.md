# Starshard Cove — design (living)

See also the original one-pager intent and the playable build.

## Pitch

Third-person collectathon on a tiny cove island. Collect **8 starshards**, chase a soft time rank, celebrate on the win screen.

## Playable build

```bash
make -C capstone/starshard-cove
```

## Scope (v1 shipped in course)

- One outdoor island mesh (scale ~0.032) + snake (~0.02)  
- Skinned player with idle/walk (`t3d_skeleton_use`)  
- Follow camera + free `camYaw` orbit; move from lagged eye  
- 8 collectibles, **box** soft wall (not radial)  
- Title / play / pause / win  
- Music + SFX (`audio_init(48000)` for Opus)  
- HUD count + timer + ranks  
- `FILTERS_RESAMPLE` + opaque clear (no VI AA edge flicker)

## Stretch (your sequel)

- Jump + anim  
- EEPROM high score  
- Second area  
- Night lighting  
- Your own player rig replacing the reference character  

## Controls

Stick move · C-left/right camera · START pause / menus

## Full capstone source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`capstone/starshard-cove/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/capstone/starshard-cove/Makefile) · [`capstone/starshard-cove/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/capstone/starshard-cove/src/main.c)

::: details `capstone/starshard-cove/Makefile`
<<< ../../../capstone/starshard-cove/Makefile{make}
:::

::: details `capstone/starshard-cove/src/main.c`
<<< ../../../capstone/starshard-cove/src/main.c{c}
:::
