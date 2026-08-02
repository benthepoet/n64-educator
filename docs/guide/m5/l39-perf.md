# L39 — Measuring performance

::: tip Goal
Put **frame time in milliseconds** on screen so “is it slow?” is a number, not a vibe.
:::

## In plain English

```c
uint64_t t0 = get_ticks_ms();
/* … update + draw … */
uint64_t t1 = get_ticks_ms();
float frame_ms = (float)(t1 - t0);
```

~**16.6 ms** ≈ 60 Hz. If you’re comfortably under budget, **stop optimizing**.

This lesson burns CPU on purpose (busy loop) so you can push the meter with the stick. Real games more often die on **RDP fill / TMEM / polycount** — still measure first.

## What you will see

```bash
source scripts/env.sh
make -C lessons/l39-perf
```

| Input | Effect |
|-------|--------|
| Stick up | More fake CPU work → higher ms |
| Stick down | Less work |
| Red clear | Over ~17 ms budget |

## Habits (with L36)

- Measure, then cut art cost (textures, overdraw, unique maps)  
- Don’t thrash TMEM  
- Profile by feel only when the meter already looks fine  

## Full lesson source

[`lessons/l39-perf/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l39-perf/Makefile) · [`lessons/l39-perf/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l39-perf/src/main.c)

::: details `lessons/l39-perf/Makefile`
<<< ../../../lessons/l39-perf/Makefile{make}
:::

::: details `lessons/l39-perf/src/main.c`
<<< ../../../lessons/l39-perf/src/main.c{c}
:::

## Next

[L35 — Capstone assembly](./l35-capstone) · [L36 — Shipping](./l36-shipping).
