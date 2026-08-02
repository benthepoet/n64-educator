# L38 — Save & high scores

::: tip Goal
Persist a **best run time** across power cycles with cartridge EEPROM and `eepromfs`.
:::

## In plain English

```text
eeprom_present()     → is there a save chip?
eepfs_init(files)    → declare tiny “files” in EEPROM
eepfs_verify_signature / wipe if foreign data
eepfs_read / write   → load & store a packed struct
while (eeprom_is_busy())  → wait for background flush
```

Makefile advertises save type to emulators/flashcarts:

```make
N64_ROM_SAVETYPE := eeprom4k   # after include common/lesson.mk
```

## What you will see

```bash
source scripts/env.sh
make -C lessons/l38-save
```

| Input | Action |
|-------|--------|
| **A** | Start a timed run; press again to finish & maybe save best |
| **START** | Pause the run timer |
| **Z** | Wipe save |

Power-cycle (or reload with save battery) — **best** should remain if EEPROM is available.

Ares: Homebrew Mode + savetype from header. If EEPROM shows `NONE`, check emulator save settings.

## Capstone

[Starshard Cove](./starshard-cove) stores **best clear time** the same way (`cove.dat` on eeprom4k).

## Full lesson source

[`lessons/l38-save/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l38-save/Makefile) · [`lessons/l38-save/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l38-save/src/main.c)

::: details `lessons/l38-save/Makefile`
<<< ../../../lessons/l38-save/Makefile{make}
:::

::: details `lessons/l38-save/src/main.c`
<<< ../../../lessons/l38-save/src/main.c{c}
:::

## Next

[L39 — Measuring performance](./l39-perf).
