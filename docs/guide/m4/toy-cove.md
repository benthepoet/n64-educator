# Module 4 checkpoint — Toy Cove

::: tip Goal
Play a complete loop: title → explore island → collect 3 starshards → win → return to title.
:::

## Build & run

```bash
source scripts/env.sh
make -C lessons/m4-toy-cove
# → m4_toy_cove.z64
```

| Control | Action |
|---------|--------|
| START | Title→Play, Play⇄Pause, Win→Title |
| Stick | Move (camera-relative) |
| C-left/right | Orbit camera |

## Skills checklist

- [ ] Camera-relative move  
- [ ] Idle/walk blend  
- [ ] Follow camera with lag  
- [ ] Sphere collect  
- [ ] Entity/spawn mindset  
- [ ] State machine  


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/m4-toy-cove/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m4-toy-cove/Makefile) · [`lessons/m4-toy-cove/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m4-toy-cove/src/main.c)

::: details `lessons/m4-toy-cove/Makefile`
<<< ../../../lessons/m4-toy-cove/Makefile{make}
:::

::: details `lessons/m4-toy-cove/src/main.c`
<<< ../../../lessons/m4-toy-cove/src/main.c{c}
:::

## Next

[Module 5 — Polish & Starshard Cove](/guide/m5/overview): audio, HUD, juice, then the full [8-shard capstone](/guide/m5/l35-capstone).
