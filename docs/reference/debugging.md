# Debugging cheat sheet

When a ROM misbehaves, use a **loop** — do not random-edit forever.

## 1. Rebuild & reload hygiene

| Step | Why |
|------|-----|
| `make -C lessons/<name> clean all` | Stale `.z64` is a common false bug (source fixed, binary old) |
| Fully reload the ROM in Ares | Some emulators keep the previous image |
| Enable **Homebrew Mode** | Needed for advanced header, EEPROM savetype, etc. |
| Confirm path | Load the `.z64` you just built |

## 2. Logging

```c
debug_init_isviewer();  /* Ares / isviewer spew */
debug_init_usblog();    /* USB (SC64, etc.) when available */
debugf("hello %d\n", x); /* goes to those sinks */
```

Course lessons already call the init helpers early. Use `debugf` liberally while hunting bugs; remove or gate noise later.

## 3. Asserts vs silent failure

```c
assertf(model != NULL, "missing rom:/island.t3dm");
```

Prefer **loud** failures (missing asset, wrong audio rate) over blue clear + HUD only.

## 4. Common failure → cause

| Symptom | Likely cause |
|---------|----------------|
| Blue/clear + text, no 3D | Matrix `set` over camera; wrong scales; missing model; forgot `skeleton_use` |
| `frequency 48000 exceeds…` | `audio_init` below Opus wav64 rate — use **48000** |
| `File not found: …sdata` | glTF convert must output under `filesystem/` so paths are `rom:/…` |
| 1px green/cyan top line | `FILTERS_RESAMPLE_ANTIALIAS` — use **`FILTERS_RESAMPLE`** + opaque clear |
| Camera/stick spin | Move used player yaw; or soft-followed `camYaw`→player yaw; or radial wall |
| Assert on boot | Read the message; fix the condition |
| Exception / freeze | NULL deref, bad DMA alignment — check log/backtrace |

## 5. Deliberate practice

[L37 — When it crashes](/guide/m0/l37-debug) fires `debugf`, `assertf`, and a hard crash on purpose so you can read each path once.

## 6. Hardware notes

Flashcarts (SC64, EverDrive): use USB loaders that surface libdragon logs when possible. Save types must match the ROM header (`N64_ROM_SAVETYPE` — see L38).
