# L27 — Drive skeletal animation from gameplay

::: tip Goal
Play **idle** when still and blend toward **walk** as speed rises — no button for “start walk.”
:::

## In plain English

Animations are data. Gameplay chooses **which clip** and **how much blend**:

```text
blend = clamp(speed, 0, 1)
update idle → skeleton A
update walk → skeleton B
skeleton = lerp(A, B, blend)
t3d_skeleton_update   // fill bone matrices (advances buffer)
t3d_skeleton_use      // REQUIRED for buffered skeletons
draw skinned
```

## What you will see

```bash
source scripts/env.sh
make -C lessons/l27-anim-drive clean all
```

Sandy **sample map** (Tiny3D `08_animation` lineage) + Quaternius **snake** character
shipped as `player_anim`. Character **idles** when still and **blends to walk** as
you push the stick. Stick-up moves **into the scene** (camera-relative, same as L26).

Clear color is warm sand (not the old blue-only bug) so a fresh ROM is obvious.

## Pitfalls

- Restarting a walk clip every frame → jitter; let it loop  
- Forgetting `t3d_skeleton_update` after blend  
- **Forgetting `t3d_skeleton_use` before `t3d_model_draw_skinned`** when using
  `t3d_skeleton_create_buffered` — HUD/text still draws, skinned mesh is missing
  (blue/sand clear only). Live `main.c` calls it every frame.  
- Zero `dt` if you pause wrong  
- **`ASSERTION FAILED: File not found: player_anim.2.sdata`** — animation streams
  (`.sdata`) must be on the ROM **and** the `.t3dm` must reference them as
  `rom:/player_anim.N.sdata`. That only happens if `gltf_to_t3d` is run with an
  output path under `filesystem/` (our `common/lesson.mk` does this). Rebuild:

  ```bash
  source scripts/env.sh
  make -C lessons/l27-anim-drive clean all
  ```

  Hand convert with e.g. `gltf_to_t3d model.glb filesystem/model.t3dm` — **not**
  `gltf_to_t3d model.glb model.t3dm` (bare paths break `asset_fopen`).

## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l27-anim-drive/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l27-anim-drive/Makefile) · [`lessons/l27-anim-drive/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l27-anim-drive/src/main.c)

::: details `lessons/l27-anim-drive/Makefile`
<<< ../../../lessons/l27-anim-drive/Makefile{make}
:::

::: details `lessons/l27-anim-drive/src/main.c`
<<< ../../../lessons/l27-anim-drive/src/main.c{c}
:::

## Next

[L28 — Follow cam](./l28-follow-cam).
