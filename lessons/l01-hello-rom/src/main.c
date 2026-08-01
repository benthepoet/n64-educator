/**
 * L01 — Hello ROM
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Prove the toolchain works: compile C → .z64 → run in Ares and see text.
 *
 * WHAT HAPPENS EACH FRAME
 * -----------------------
 *   display_get()     wait for a free framebuffer (display-paced; see L02 vsync)
 *   rdpq_attach()     all following RDP draws go to that buffer
 *   rdpq_clear()      fill background color
 *   rdpq_text_print() draw strings with a registered font
 *   rdpq_detach_show() present when the RDP is done
 *
 * There is no "engine" yet — just an infinite loop redrawing the same message.
 *
 * BUILD: make -C lessons/l01-hello-rom
 * DOCS:  docs/guide/m0/l01-hello-rom.md
 */

#include <libdragon.h>

int main(void)
{
    /*
     * display_init(resolution, depth, num_buffers, gamma, filters)
     *   320x240  — classic friendly homebrew res
     *   16 bpp   — 5:5:5:1 color, less RAM than 32 bpp
     *   2 buffers — double buffering (draw one while showing the other)
     */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);

    /* RDPQ = helpers that talk to the Reality Display Processor. */
    rdpq_init();

    /*
     * Register a font under numeric id 1.
     * Later rdpq_text_print(..., font_id=1, ...) uses this.
     * Built-in debug font needs no assets on the ROM.
     */
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    /* N64 games almost never exit main — loop forever. */
    while (1) {
        /* Block until a framebuffer is free (ties us to display timing). */
        surface_t *disp = display_get();

        /*
         * NULL z-buffer: we are 2D-only this lesson.
         * 3D lessons pass display_get_zbuf() as the second argument.
         */
        rdpq_attach(disp, NULL);

        /* Background color: dark blue (RGBA 0–255). */
        rdpq_clear((color_t){ .r = 16, .g = 24, .b = 48, .a = 255 });

        /*
         * rdpq_text_print(style, font_id, x, y, string)
         *   x,y are in pixels from the top-left of the framebuffer.
         */
        rdpq_text_print(NULL, 1, 40, 80, "N64 Educator");
        rdpq_text_print(NULL, 1, 40, 100, "L01 — Hello ROM");
        rdpq_text_print(NULL, 1, 40, 140, "If you can read this,");
        rdpq_text_print(NULL, 1, 40, 156, "your toolchain works!");

        /* Finish RDP work and queue this buffer for display. */
        rdpq_detach_show();
    }
}
