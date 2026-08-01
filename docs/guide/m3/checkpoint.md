# Module 3 checkpoint — Asset lab

::: tip Goal
Prove the art pipeline: view **starshard**, **platform**, **island**, **player_static**, and **player_anim** on hardware/emulator, and know how to replace them with your own exports.
:::

## Build & run

```bash
source scripts/env.sh
./scripts/build-course-assets.sh   # optional regenerate
make -C lessons/m3-asset-lab
# → lessons/m3-asset-lab/m3_lab.z64
```

| Input | Action |
|-------|--------|
| D-pad L/R | Cycle props |
| Stick | Orbit |
| C-up/dn | Dolly |
| A | Idle ↔ walk blend (animated prop only) |

## Scales in this lab

The lab applies a **per-prop scale** so huge integer `t3dm` verts fit the camera
(e.g. island ~0.032, `player_anim` ~0.02, starshard ~0.025). That matches Module 4
gameplay ROMs. Viewing at scale `1` often looks empty.

Animated prop: call **`t3d_skeleton_use`** before `t3d_model_draw_skinned`, and
**`matrix_push` / `pop`** around the draw.

## Self-check

- [ ] All five props appear (or you understand any missing load)  
- [ ] Island shows spatial tinting (vertex paint)  
- [ ] Animated character idles; A moves toward walk  
- [ ] You can explain glb → inject/Fast64 → t3dm → `rom:/`  

## Replace a prop with yours

1. Export `mystar.glb` from Blender (checklist L21).  
2. Copy to `lessons/m3-asset-lab/assets/starshard.glb` **or** convert to `starshard.t3dm` and overwrite.  
3. Include textures/png if any.  
4. `make -C lessons/m3-asset-lab clean && make -C lessons/m3-asset-lab`  
5. View in Ares.

## Module 3 done when

You are not afraid of Blender’s viewport, you can export *something* into the lab, and you understand why Fast64/custom properties matter for Tiny3D.


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/m3-asset-lab/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m3-asset-lab/Makefile) · [`lessons/m3-asset-lab/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m3-asset-lab/src/main.c)

::: details `lessons/m3-asset-lab/Makefile`
<<< ../../../lessons/m3-asset-lab/Makefile{make}
:::

::: details `lessons/m3-asset-lab/src/main.c`
<<< ../../../lessons/m3-asset-lab/src/main.c{c}
:::

## Next

[Module 4 — Gameplay systems](/guide/m4/overview): move, drive anims, follow cam, collide, entities, state machine → [Toy Cove](/guide/m4/toy-cove).
