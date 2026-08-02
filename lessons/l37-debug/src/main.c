/**
 * L37 — When it crashes (debugging workflow)
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * When a ROM fails, you need a *loop*, not panic:
 *   1. Rebuild clean (stale .z64 is a common false bug)
 *   2. Reload the ROM; enable Homebrew Mode in Ares
 *   3. Read on-screen text / assert message
 *   4. Read emulator / USB logs (debugf)
 *   5. Narrow: last change, missing asset, bad rate, matrix stack
 *
 * This lesson deliberately:
 *   - Prints debugf lines every second (see Ares / isviewer log)
 *   - A  → soft assert message path (assertf) — use carefully
 *   - B  → intentional NULL deref crash so you can read the exception screen
 *   - Z  → log a heap of state with debugf
 *
 * BUILD: make -C lessons/l37-debug
 * DOCS:  docs/guide/m0/l37-debug.md
 * REF:   docs/reference/debugging.md
 */

#include <libdragon.h>
#include <stdio.h>

int main(void)
{
    /* Always first: logging sinks before anything can assert. */
    debug_init_isviewer();
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    debugf("L37: boot ok — debugf is alive\n");

    float t = 0.f;
    float last_log = 0.f;
    int frame = 0;
    char line[72];

    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        /* Crude dt from ticks (fine for a debug toy). */
        static uint64_t last_ms;
        uint64_t now_ms = get_ticks_ms();
        float dt = 0.016f;
        if (last_ms != 0) {
            dt = (float)(now_ms - last_ms) / 1000.f;
            if (dt < 0.f) {
                dt = 0.f;
            }
            if (dt > 0.1f) {
                dt = 0.1f;
            }
        }
        last_ms = now_ms;
        t += dt;
        frame++;

        if (t - last_log >= 1.f) {
            last_log = t;
            debugf("L37: heartbeat t=%.1f frame=%d\n", t, frame);
        }

        if (pressed.z) {
            debugf("L37: dump — t=%.2f frame=%d stick later\n", t, frame);
        }

        /*
         * A: assertf — stops with a clear on-screen message in debug builds.
         * Prefer this over silent wrong behavior (missing models, bad rates).
         */
        if (pressed.a) {
            debugf("L37: about to assertf (press A intentionally)\n");
            assertf(false, "L37 demo assert: you pressed A. This is how bad rates / missing assets fail loudly.");
        }

        /*
         * B: hard crash — exception handler dumps PC/backtrace via debugf
         * and usually freezes the screen. Read the log / exception text.
         */
        if (pressed.b) {
            debugf("L37: about to NULL-deref (press B intentionally)\n");
            volatile int *p = NULL;
            *p = 1;
        }

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 20, .g = 28, .b = 48, .a = 255 });

        rdpq_text_print(NULL, 1, 12, 16, "L37 — When it crashes");
        rdpq_text_print(NULL, 1, 12, 40, "debugf heartbeat every 1s (check log)");
        rdpq_text_print(NULL, 1, 12, 64, "A = assertf demo (loud fail)");
        rdpq_text_print(NULL, 1, 12, 80, "B = crash demo (exception screen)");
        rdpq_text_print(NULL, 1, 12, 96, "Z = debugf state dump");
        snprintf(line, sizeof(line), "t=%.1fs  frame=%d", t, frame);
        rdpq_text_print(NULL, 1, 12, 128, line);
        rdpq_text_print(NULL, 1, 12, 160, "Always: clean rebuild + reload ROM");
        rdpq_text_print(NULL, 1, 12, 176, "Ares: enable Homebrew Mode");
        rdpq_text_print(NULL, 1, 12, 208, "See docs/reference/debugging.md");

        rdpq_detach_show();
    }
}
