# L02 — Game loop & display

::: tip Goal
Turn the infinite redraw into an explicit **update → render** loop, see time pass via a frame counter and a pulsing clear color, and understand that **`display_get` is the vsync-style wait** on the N64 (VI + double buffering).
:::

## What you will see

Text plus a **slowly pulsing** blue-green background and a live **frame counter**.

## Why it matters

Games are loops:

1. **Update** — simulate (time, input, physics, AI)
2. **Render** — draw the world for this frame
3. **Present** — show the framebuffer; wait for the next slot

L01 did all of that implicitly. L02 names the phases so later lessons (input, 3D, gameplay) have a place to plug in.

## Source walkthrough

### Frame counter in update

```c
    uint32_t frame = 0;

    while (1) {
        /* -------- Update -------- */
        frame++;
        uint8_t pulse = (uint8_t)((frame / 2) & 0xFF);
        /* ... derive bg color from pulse ... */
```

No delta-time yet — we count **presented frames**. That is good enough for demos; real gameplay later can use timers (`get_ticks_ms`, etc.).

### Render still owns the GPU work

```c
        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear(bg);
        rdpq_text_print(/* ... */);
        rdpq_detach_show();
```

`display_get()` blocks until a framebuffer is free. With **2 buffers**, that roughly paces you to the display’s refresh when the RDP finishes in time.

### Mental model

```text
   ┌─────────────┐
   │   Update    │  CPU: game logic
   └──────┬──────┘
          ▼
   ┌─────────────┐
   │   Render    │  CPU builds RDP commands → RDP draws
   └──────┬──────┘
          ▼
   ┌─────────────┐
   │   Present   │  show buffer; wait for next free buffer
   └──────┬──────┘
          └──────────► loop
```

On N64, the **RDP** does the heavy pixel work; your CPU should not busy-spin on pixels in software if the RDP can do it (we already clear via RDPQ).

## Vsync on the N64 (important)

::: info Same idea as older consoles
The N64 **does** sync to the display, just like classic “wait for vblank” loops on older systems. The hardware unit is the **VI (Video Interface)**; libdragon’s double-buffered display API is how we participate in that timing.
:::

### What “vsync” meant on older machines

On many 8‑ and 16‑bit consoles you waited for **vertical blank** (vblank): the video chip finished scanning a frame, raised a flag or interrupt, and only then you swapped buffers or rewrote VRAM. That kept the game loop **paced by the TV** (~60 Hz NTSC / ~50 Hz PAL) and avoided tearing.

### What the N64 has

The **VI** scans a framebuffer out to the TV and generates **vertical retrace / field** events. A normal game:

1. Draws into a **back** buffer (usually via the RDP).
2. At (or near) vertical blank, the VI is pointed at the completed buffer and the next frame begins.

There is not always a single function literally named `vsync()` in every API, but the **concept is the same**: do not free-run the loop as fast as the CPU can go; **present on the display’s schedule**.

### How that maps to this lesson

| Classic pattern | This lesson (libdragon) |
|-----------------|-------------------------|
| Wait for vblank | `display_get()` blocks until a **free framebuffer** is available |
| Flip / swap buffers | `rdpq_detach_show()` finishes RDP work and **queues the buffer for display** |
| Double buffering | `display_init(..., 2, ...)` — two framebuffers |
| Tear if you update mid-scan | Avoided by not showing a buffer still being drawn |

So when you see `display_get()` stall until the counter advances at a steady rate, you are not stuck — you are **waiting on the display pipeline** (VI + buffer ownership). That is the N64 homebrew equivalent of “wait for vsync, then flip.”

::: tip Mental model to keep
**VI paces the show. Double buffering gives you a private canvas. `display_get` → draw → `rdpq_detach_show` is the vsync-friendly loop.**
:::

### Caveats (know, don’t overthink yet)

- One loop iteration is **usually** one displayed frame when work fits the budget; if the CPU or RDP runs long, you can **drop** or stretch timing.
- We still count **frames**, not milliseconds. Later lessons may use timers (`get_ticks_ms`, etc.) when movement needs real delta time.
- PAL vs NTSC rates differ; we are not handling region logic here.
- You can go deeper later with VI interrupts and explicit retrace handling — Module 0’s hardware tour will name the chips; this lesson only needs the **vsync idea**.

## Build & run

```bash
make -C lessons/l02-game-loop
# → lessons/l02-game-loop/l02_loop.z64
```

Open in Ares (Homebrew mode). Confirm the number climbs and the background breathes.

## Exercises

1. Change how fast `pulse` moves (`frame / 2` → `/ 4` or use `frame` directly).
2. Print `frame / 60` as a rough “seconds” estimate (not exact — discuss why).
3. Sketch where **controller read** will go in L03 (hint: update, not render).

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Counter stuck at 0 | You are not rebuilding / running the new ROM |
| Flicker / tearing worries | Double-buffering helps; focus on structure for now |
| Color looks wrong | `color_t` fields are `r,g,b,a` 0–255 |


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l02-game-loop/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l02-game-loop/Makefile) · [`lessons/l02-game-loop/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l02-game-loop/src/main.c)

::: details `lessons/l02-game-loop/Makefile`
<<< ../../../lessons/l02-game-loop/Makefile{make}
:::

::: details `lessons/l02-game-loop/src/main.c`
<<< ../../../lessons/l02-game-loop/src/main.c{c}
:::

## What you learned

- Update vs render vs present
- Frame pacing via `display_get`
- **N64 vsync**: VI-driven display timing; double-buffered present is the classic vblank idea under a modern API
- Driving visuals from simulation state (`frame`, `bg`)

## Next

[L03 — Controllers](/guide/m0/l03-controllers) reads the stick and buttons inside **update**.
