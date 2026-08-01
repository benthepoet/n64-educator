# L32 — Audio

::: tip Goal
Initialize the mixer, play **SFX** on button presses, and toggle **XM music**. Never forget `mixer_try_play()`.
:::

## In plain English

N64 audio is mixed on the **RSP**. Your CPU:

1. Loads samples from ROM (`wav64_load`, `xm64player_open`)  
2. Starts voices on mixer channels (`wav64_play`, `xm64player_play`)  
3. Every frame, calls **`mixer_try_play()`** so buffers keep flowing  

## What you will see

```bash
make -C lessons/l32-audio
```

| Input | Sound |
|-------|--------|
| A | Collect SFX |
| B | UI blip |
| START | Win jingle |
| Z | Music on/off |

## Core code shape

```c
audio_init(44100, 4);
mixer_init(16);
wav64_init_compression(3);

wav64_t *sfx = wav64_load("rom:/collect.wav64", NULL);
xm64player_t music;
xm64player_open(&music, "rom:/music.xm64");
xm64player_set_loop(&music, true);

// each frame, after input/render:
mixer_try_play();
```

## Volume etiquette

Keep music under SFX (~0.5–0.6). Avoid stacking five loud one-shots on the same channel without planning.

## Exercises

1. Map collect SFX to a different button.  
2. Lower music volume with `xm64player_set_vol`.  


## Full lesson source

The blocks below are **imported from the real repository files** at build time
(VitePress `<<<` snippets). They are not hand-copied into this markdown.

[`lessons/l32-audio/Makefile`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l32-audio/Makefile) · [`lessons/l32-audio/src/main.c`](https://github.com/benthepoet/n64-educator/blob/master/lessons/l32-audio/src/main.c)

::: details `lessons/l32-audio/Makefile`
<<< ../../../lessons/l32-audio/Makefile{make}
:::

::: details `lessons/l32-audio/src/main.c`
<<< ../../../lessons/l32-audio/src/main.c{c}
:::

## Next

[L33 — HUD](./l33-hud).
