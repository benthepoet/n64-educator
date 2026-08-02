/**
 * L39 — Measuring performance
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * "Profile by feel" is fine until it isn't. Measure:
 *   frame_ms = get_ticks_ms() delta between presents
 *   show ms + estimated FPS on the HUD
 *
 * Stick up = more fake CPU work (busy loop) so frame time rises.
 * Stick down = less work. Target ~16.6 ms for 60 Hz feel.
 *
 * BUILD: make -C lessons/l39-perf
 * DOCS:  docs/guide/m5/l39-perf.md
 */

#include <libdragon.h>
#include <stdio.h>

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    int work = 5000; /* busy-loop iterations per frame */
    float frame_ms = 16.f;
    float avg_ms = 16.f;
    char line[80];

    for (;;) {
        uint64_t t0 = get_ticks_ms();

        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        /* Stick Y adjusts artificial load */
        if (in.stick_y > 20) {
            work += 200;
        }
        if (in.stick_y < -20) {
            work -= 200;
        }
        if (work < 0) {
            work = 0;
        }
        if (work > 200000) {
            work = 200000;
        }

        /* Fake CPU cost (not RDP) — enough to push frame time. */
        volatile int sink = 0;
        for (int i = 0; i < work; i++) {
            sink += i;
        }
        (void)sink;

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);

        /* Tint red when we blow a 16.6 ms budget. */
        color_t bg = (color_t){ .r = 24, .g = 32, .b = 48, .a = 255 };
        if (frame_ms > 17.f) {
            bg = (color_t){ .r = 48, .g = 24, .b = 24, .a = 255 };
        }
        rdpq_clear(bg);

        float fps = (frame_ms > 0.1f) ? (1000.f / frame_ms) : 0.f;
        rdpq_text_print(NULL, 1, 12, 14, "L39 — Measuring performance");
        snprintf(line, sizeof(line), "frame: %5.1f ms   avg: %5.1f ms",
                 (double)frame_ms, (double)avg_ms);
        rdpq_text_print(NULL, 1, 12, 40, line);
        snprintf(line, sizeof(line), "est FPS: %5.1f   work: %d",
                 (double)fps, work);
        rdpq_text_print(NULL, 1, 12, 56, line);
        rdpq_text_print(NULL, 1, 12, 88, "Stick up = more CPU work");
        rdpq_text_print(NULL, 1, 12, 104, "Stick down = less work");
        rdpq_text_print(NULL, 1, 12, 136, "Budget ~16.6 ms @ 60 Hz");
        rdpq_text_print(NULL, 1, 12, 152, "Red clear = over budget");
        rdpq_text_print(NULL, 1, 12, 184, "If under budget: stop optimizing.");
        rdpq_text_print(NULL, 1, 12, 208, "Art/TMEM still dominate real games");

        rdpq_detach_show();

        uint64_t t1 = get_ticks_ms();
        frame_ms = (float)(t1 - t0);
        if (frame_ms < 1.f) {
            frame_ms = 1.f; /* avoid div0; vsync may pad */
        }
        avg_ms = avg_ms * 0.9f + frame_ms * 0.1f;
    }
}
