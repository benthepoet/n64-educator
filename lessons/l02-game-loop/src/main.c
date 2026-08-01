/**
 * L02 — Game loop & display
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Name the phases of a game frame and see time pass.
 *
 *   UPDATE  — change simulation state (here: frame counter, pulse color)
 *   RENDER  — draw that state into a framebuffer
 *   PRESENT — show it (rdpq_detach_show); next display_get waits its turn
 *
 * VSYNC / VI (important)
 * ----------------------
 * display_get() blocks until a free framebuffer exists. With double buffering
 * that paces you to the display pipeline — the N64 equivalent of waiting for
 * vblank on older consoles. The VI (Video Interface) scans frames out to the
 * TV. See docs/guide/m0/l02-game-loop.md for the full story.
 *
 * BUILD: make -C lessons/l02-game-loop
 */

#include <libdragon.h>
#include <stdio.h>

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    uint32_t frame = 0; /* simulation state: how many frames we have presented */

    while (1) {
        /* -------- UPDATE --------
         * Pure CPU work. No drawing yet.
         * Later lessons put input, physics, AI here.
         */
        frame++;

        /*
         * Build a slow pulse 0..128..0 from the frame counter.
         * (frame/2) slows the sawtooth; fold it into a triangle wave.
         */
        uint8_t pulse = (uint8_t)((frame / 2) & 0xFF);
        if (pulse > 128) {
            pulse = (uint8_t)(255 - pulse);
        }

        /* Map pulse into a readable blue-green background. */
        color_t bg = {
            .r = 8,
            .g = (uint8_t)(24 + pulse / 2),
            .b = (uint8_t)(48 + pulse / 3),
            .a = 255,
        };

        char line[64];
        snprintf(line, sizeof(line), "Frame: %lu", (unsigned long)frame);

        /* -------- RENDER + PRESENT --------
         * display_get may wait (display pacing / vsync-style).
         */
        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);

        rdpq_clear(bg);

        rdpq_text_print(NULL, 1, 40, 70, "L02 — Game loop");
        rdpq_text_print(NULL, 1, 40, 100, line);
        rdpq_text_print(NULL, 1, 40, 130, "Update then render,");
        rdpq_text_print(NULL, 1, 40, 146, "once per displayed frame.");

        rdpq_detach_show();
    }
}
