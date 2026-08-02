# L37 — When it crashes

::: tip Goal
Build a **debugging habit**: clean rebuild, reload, logs, asserts, and reading a crash.
:::

## In plain English

Shipping ROMs fail. Pros don’t guess — they narrow:

1. Rebuild clean + reload the ROM  
2. Homebrew Mode on  
3. Read on-screen text / assert  
4. Read `debugf` logs  
5. Check the common failure table ([debugging reference](/reference/debugging))

## What you will see

```bash
source scripts/env.sh
make -C lessons/l37-debug
```

| Input | Effect |
|-------|--------|
| (idle) | `debugf` heartbeat every ~1 s in the log |
| **A** | `assertf` demo (loud stop) |
| **B** | intentional crash / exception screen |
| **Z** | extra `debugf` dump |

## Why this is early

You will hit blue screens and asserts through Modules 2–5. Practice the tools **before** the 3D soup gets thick.

## Full lesson source

[`lessons/l37-debug/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l37-debug/Makefile) · [`lessons/l37-debug/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l37-debug/src/main.c)

::: details `lessons/l37-debug/Makefile`
<<< ../../../lessons/l37-debug/Makefile{make}
:::

::: details `lessons/l37-debug/src/main.c`
<<< ../../../lessons/l37-debug/src/main.c{c}
:::

## Next

[Module 0 checkpoint](./checkpoint) if you haven’t finished foundations — or continue the course. Keep [debugging](/reference/debugging) bookmarked.
