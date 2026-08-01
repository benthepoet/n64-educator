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
make -C lessons/l27-anim-drive
```

Reference character walks when you push the stick; idles when you stop.

## Pitfalls

- Restarting a walk clip every frame → jitter; let it loop  
- Forgetting `t3d_skeleton_update` after blend  
- Zero `dt` if you pause wrong  


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
