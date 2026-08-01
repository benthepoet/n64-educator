# L01 — Hello ROM

::: tip Goal
Build your first libdragon ROM and see text on screen in Ares. That proves the toolchain, Makefile, and emulator path work before anything harder.
:::

## What you will see

A dark blue screen with:

```text
N64 Educator
L01 — Hello ROM
If you can read this,
your toolchain works!
```

## Project layout

```text
lessons/l01-hello-rom/
  Makefile
  src/main.c
```

The Makefile sets `ROMNAME` and includes the shared fragment:

```make
ROMNAME   := l01_hello
ROM_TITLE := "L01 Hello"

include ../../common/lesson.mk
```

`common/lesson.mk` expects `N64_INST`, includes libdragon’s `n64.mk`, compiles every `.c` under `src/`, and links `l01_hello.z64`.

## Source walkthrough

### Includes and `main`

```c
#include <libdragon.h>

int main(void)
{
```

Almost every libdragon program starts this way. Subsystems are initialized explicitly.

### Display + RDPQ

```c
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
```

| Call | Meaning |
|------|---------|
| `display_init` | Resolution, color depth, framebuffer count (here: 2 for double-buffering) |
| `rdpq_init` | Reality Display Processor command queue helpers — we use them for clear + text |

320×240 at 16 bpp is a common, friendly mode for development.

### Font

```c
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
```

Font **id 1** is a built-in debug font. Later lessons use nicer fonts; this one needs no assets.

### The loop

```c
    while (1) {
        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);

        rdpq_clear((color_t){ .r = 16, .g = 24, .b = 48, .a = 255 });

        rdpq_text_print(NULL, 1, 40, 80, "N64 Educator");
        /* ... more lines ... */

        rdpq_detach_show();
    }
```

| Step | Role |
|------|------|
| `display_get` | Wait for a free framebuffer |
| `rdpq_attach` | Direct RDPQ drawing at that surface (`NULL` z-buffer for 2D-only) |
| `rdpq_clear` | Fill background color |
| `rdpq_text_print` | Draw a string (font id, x, y) |
| `rdpq_detach_show` | Finish and present when ready |

There is no “engine” yet — just an infinite loop that redraws the same frame. Module 0 later adds timing, input, and assets.

## Build & run

From the **repository root**:

```bash
# Native toolchain
export N64_INST=/opt/libdragon   # your path
make -C lessons/l01-hello-rom

# Or Docker
libdragon make -C lessons/l01-hello-rom
```

Open `lessons/l01-hello-rom/l01_hello.z64` in **Ares** with Homebrew mode enabled.

### Clean

```bash
make -C lessons/l01-hello-rom clean
```

## Exercises

1. Change the clear color and rebuild. Can you get a warm sunset background?
2. Move the text by editing the `x, y` arguments to `rdpq_text_print`.
3. Add a fourth line with your name.

## Troubleshooting

| Problem | Fix |
|---------|-----|
| `N64_INST is not set` | See [Setup](/guide/setup) |
| Compile errors about missing headers | libdragon not installed / wrong branch |
| Emulator shows nothing / crashes | Use Ares; enable Homebrew mode |
| Text missing but color OK | Font registration failed — compare your `main.c` to the lesson file |



## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l01-hello-rom/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l01-hello-rom/Makefile) · [`lessons/l01-hello-rom/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l01-hello-rom/src/main.c)

::: details `lessons/l01-hello-rom/Makefile`
<<< ../../../lessons/l01-hello-rom/Makefile{make}
:::

::: details `lessons/l01-hello-rom/src/main.c`
<<< ../../../lessons/l01-hello-rom/src/main.c{c}
:::

## What you learned

- libdragon project shape (`Makefile` + `src/`)
- Display init and a minimal present loop
- RDPQ clear + built-in text

## Next

[L02 — Game loop & display](/guide/m0/l02-game-loop)
