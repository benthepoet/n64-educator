/**
 * L32 — Audio (mixer + WAV SFX + XM music)
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Make the N64 play sounds. Audio is mixed on the RSP; your job is to:
 *   1. Init audio + mixer once
 *   2. Load wav64 / xm64 from the ROM
 *   3. Start playback on a channel when something happens
 *   4. Call mixer_try_play() EVERY FRAME (or audio glitches / stops)
 *
 * CHANNELS
 * --------
 * Think of mixer channels as simultaneous "voice slots".
 * We reserve:
 *   0 = SFX A (collect)
 *   1 = SFX B (ui)
 *   2+ = music (XM may use several starting at 2)
 *
 * ASSETS (converted at build by common/lesson.mk)
 * -----------------------------------------------
 *   assets/collect.wav → filesystem/collect.wav64
 *   assets/ui.wav      → ui.wav64
 *   assets/win.wav     → win.wav64
 *   assets/music.xm    → music.xm64
 *
 * CONTROLS: A collect, B ui, START win, Z music toggle
 * BUILD:    make -C lessons/l32-audio
 * DOCS:     docs/guide/m5/l32-audio.md
 */

#include <libdragon.h>
#include <stdio.h>

/* collect is stereo (0+1); win/ui mono; XM (8 ch) from CH_BGM */
#define CH_COLLECT 0
#define CH_UI      2
#define CH_WIN     3
#define CH_BGM     4

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    /* Need DFS before rom:/ audio paths work. */
    dfs_init(DFS_DEFAULT_LOCATION);

    /*
     * audio_init(output_hz, num_buffers)
     * mixer_init(max_channels)
     * wav64_init_compression(level) must match how files were converted
     *   (we use --wav-compress 3 = Opus in the Makefile rule).
     *
     * Opus wav64 is stored at 48 kHz. audio_init must be >= that rate or
     * wav64_play asserts: "frequency 48000 exceeds configured limit …".
     */
    audio_init(48000, 4);
    mixer_init(16);
    wav64_init_compression(3);

    /* Load SFX into RAM-ish streaming structures. NULL = default load params. */
    wav64_t *sfx_collect = wav64_load("rom:/collect.wav64", NULL);
    wav64_t *sfx_ui = wav64_load("rom:/ui.wav64", NULL);
    wav64_t *sfx_win = wav64_load("rom:/win.wav64", NULL);

    /* XM music player — multi-channel tracker music, great size on cartridge. */
    xm64player_t music;
    xm64player_open(&music, "rom:/music.xm64");
    xm64player_set_loop(&music, true); /* restart when the song ends */
    bool music_on = false;

    char line[64];
    int plays = 0; /* how many one-shots we've fired (for the HUD) */

    while (1) {
        joypad_poll();
        /* "Pressed" = edge this frame (not held). Perfect for one-shots. */
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (pressed.a && sfx_collect) {
            /* Play on channel 0. Starting again interrupts the previous SFX on 0. */
            wav64_play(sfx_collect, CH_COLLECT);
            plays++;
        }
        if (pressed.b && sfx_ui) {
            wav64_play(sfx_ui, CH_UI);
            plays++;
        }
        if (pressed.z) {
            music_on = !music_on;
            if (music_on) {
                /* first_ch = CH_BGM; XM may occupy several channels from here. */
                xm64player_play(&music, CH_BGM);
            } else {
                xm64player_stop(&music);
            }
        }
        if (pressed.start && sfx_win) {
            wav64_play(sfx_win, CH_WIN);
        }

        /* Simple 2D UI — no Tiny3D this lesson. */
        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 20, .g = 24, .b = 40, .a = 255 });
        rdpq_text_print(NULL, 1, 20, 40, "L32 — Audio");
        rdpq_text_print(NULL, 1, 20, 70, "A collect  B ui  START win");
        rdpq_text_print(NULL, 1, 20, 90, "Z music on/off");
        snprintf(line, sizeof(line), "plays=%d  music=%s", plays, music_on ? "ON" : "OFF");
        rdpq_text_print(NULL, 1, 20, 120, line);
        rdpq_text_print(NULL, 1, 20, 160, "mixer_try_play every frame");
        rdpq_detach_show();

        /*
         * CRITICAL: mix audio for any free output buffer.
         * Call this once per loop even if nothing is playing.
         * Capstone and all game ROMs do the same at the end of the frame.
         */
        mixer_try_play();
    }
}
