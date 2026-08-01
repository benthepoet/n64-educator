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
draw skinned
```

## What you will see

```bash
source scripts/env.sh
make -C lessons/l27-anim-drive clean all
```

Sandy sample map + Quaternius reference character (same lineage as Tiny3D’s
`08_animation` example). Character **idles** when still and **blends to walk**
as you push the stick.

Clear color is warm sand (not blue) so you can tell you have a fresh ROM.

## Pitfalls

- Restarting a walk clip every frame → jitter; let it loop  
- Forgetting `t3d_skeleton_update` after blend  
- Zero `dt` if you pause wrong  
- **`ASSERTION FAILED: File not found: player_anim.2.sdata`** — animation streams (`.sdata`) must be on the ROM **and** the `.t3dm` must reference them as `rom:/player_anim.N.sdata`. That only happens if `gltf_to_t3d` is run with an output path under `filesystem/` (our `common/lesson.mk` does this). Rebuild from a clean lesson tree:

  ```bash
  source scripts/env.sh
  make -C lessons/l27-anim-drive clean all
  ```

  If you convert models by hand, use e.g. `gltf_to_t3d model.glb filesystem/model.t3dm` — not `gltf_to_t3d model.glb model.t3dm` (bare paths break `asset_fopen`).


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
