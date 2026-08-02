# Module 0 checkpoint

::: tip Goal
One small ROM that proves you can build, run, read input, and draw a ROM-packed sprite. If this works, foundations are solid.
:::

## What you will see

- Text: count value  
- **A** increments (edge-triggered)  
- **B** resets  
- Up to 12 star sprites drawn for the current count  

## Skills exercised

| Skill | Lesson |
|-------|--------|
| Toolchain + ROM | L01 |
| Update / render + vsync-paced present | L02 |
| Joypad poll + pressed edges | L03 |
| DFS sprite | L04 |
| Mental model of the machine | L05 |
| Debugging loop (any time) | [L37](./l37-debug) · [reference](/reference/debugging) |

## Build & run

```bash
source scripts/env.sh
make -C lessons/m0-checkpoint
# → lessons/m0-checkpoint/m0_checkpoint.z64
```

Open in Ares (Homebrew mode). Map controls if needed.

## Self-check

- [ ] ROM builds without errors  
- [ ] Ares shows the UI  
- [ ] A increments once per press (not while held forever spamming — we use **pressed**)  
- [ ] B resets to 0  
- [ ] Stars appear as the count rises  

## Optional exercises

1. Change the star art in `lessons/m0-checkpoint/assets/star.png`.  
2. Cap display at 8 stars but allow the counter to go higher (HUD vs icons).  
3. Require **Start** to begin counting (simple two-state: title → play).


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/m0-checkpoint/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m0-checkpoint/Makefile) · [`lessons/m0-checkpoint/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/m0-checkpoint/src/main.c)

::: details `lessons/m0-checkpoint/Makefile`
<<< ../../../lessons/m0-checkpoint/Makefile{make}
:::

::: details `lessons/m0-checkpoint/src/main.c`
<<< ../../../lessons/m0-checkpoint/src/main.c{c}
:::

## Next

[Module 1 — 3D Literacy](/guide/m1/overview).
