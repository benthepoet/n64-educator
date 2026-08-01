# L03 — Controllers

::: tip Goal
Read the N64 controller every frame, apply a stick **deadzone**, and display buttons and stick values. Input belongs in **update**, not render.
:::

## What you will see

Live text for port 1:

- Connection status  
- Stick X/Y (after deadzone)  
- A / B / Z / Start / L / R held state  
- A short flash line when **A** is newly pressed (edge detect)

In **Ares**, map keyboard/gamepad to N64 controls if the stick does nothing.

## API pattern (libdragon joypad)

Once per project:

```c
joypad_init();
```

Once per frame, at the start of **update**:

```c
joypad_poll();
```

Then read what you need:

```c
joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
```

| Call | Meaning |
|------|---------|
| `joypad_get_inputs` | Current stick + button **held** state |
| `joypad_get_buttons_pressed` | Buttons that went **down this frame** (edges) |
| `joypad_get_buttons_released` | Buttons that went **up this frame** |
| `joypad_get_style` | What is plugged in (`NONE`, `N64`, …) |

::: warning Always poll first
Call `joypad_poll()` **once** per frame before any `joypad_get_*`. Otherwise you re-read stale or inconsistent data.
:::

## Deadzone

Raw stick values jitter around zero. A small deadzone keeps “idle” truly idle:

```c
#define STICK_DEADZONE 8

static int apply_deadzone(int v)
{
    if (v > -STICK_DEADZONE && v < STICK_DEADZONE)
        return 0;
    return v;
}
```

Later 3D movement (Module 4) will use the same idea, then normalize into a direction vector.

## Held vs pressed

| Kind | Use for |
|------|---------|
| **Held** (`in.btn.a`) | Continuous actions (aim, crouch) |
| **Pressed** (`pressed.a`) | One-shots (jump, menu confirm, “+1 count”) |

The checkpoint after L04 uses **pressed** so holding A does not spam the counter every frame… wait — actually if we only use pressed, holding does not repeat. Good for collect / UI.

## Where it sits in the loop

```text
while (1) {
    // UPDATE
    joypad_poll();
    // read stick / buttons → mutate game state

    // RENDER (vsync-paced)
    display_get();
    // draw using current state
    rdpq_detach_show();
}
```

Recall [L02](/guide/m0/l02-game-loop): `display_get` is the display-paced wait (VI / vsync-style). Input is sampled once per displayed frame in this simple structure.

## Build & run

```bash
source scripts/env.sh
make -C lessons/l03-controllers
# → lessons/l03-controllers/l03_pad.z64
```

## Exercises

1. Draw a small crosshair that moves with the stick (clamped to the screen).
2. Increment a counter only on **Start** pressed (edge), not held.
3. Print whether a Rumble Pak is present (`joypad_get_accessory_type`) — optional stretch.

## Troubleshooting

| Problem | Fix |
|---------|-----|
| Everything zero | Emulator input not mapped; check Ares controller settings |
| Stick never centers | Raise `STICK_DEADZONE` slightly |
| A fires every frame | You used **held** instead of **pressed** for a one-shot |

## What you learned

- `joypad_init` / `joypad_poll` / `joypad_get_inputs`
- Deadzone on analog sticks
- Held vs edge-triggered buttons
- Input belongs in **update**

## Next

[L04 — Assets on ROM (DFS)](/guide/m0/l04-dfs) packs art into the cartridge image and loads it at runtime.
