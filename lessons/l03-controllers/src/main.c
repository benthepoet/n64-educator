/**
 * L03 — Controllers
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Read the N64 pad every frame and show stick + buttons on screen.
 *
 * RULES YOU MUST REMEMBER
 * -----------------------
 * 1. joypad_init() once at startup.
 * 2. joypad_poll() once per frame *before* any joypad_get_*.
 * 3. "Held" (inputs.btn.a) vs "Pressed this frame" (get_buttons_pressed)
 *    — use pressed for one-shots (jump, menu confirm, +1 count).
 * 4. Stick needs a deadzone or noise becomes random movement later.
 *
 * INPUT LIVES IN UPDATE, NOT RENDER
 * ---------------------------------
 * Read the pad, store what you need, then draw. Don't call joypad_poll
 * in the middle of drawing.
 *
 * BUILD: make -C lessons/l03-controllers
 * DOCS:  docs/guide/m0/l03-controllers.md
 */

#include <libdragon.h>
#include <stdio.h>
#include <stdlib.h>

/* Raw stick is roughly -80..+80; ignore small noise around center. */
#define STICK_DEADZONE 8

static int apply_deadzone(int v)
{
    if (v > -STICK_DEADZONE && v < STICK_DEADZONE) {
        return 0;
    }
    return v;
}

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    /* Enable the joypad subsystem (all four ports). */
    joypad_init();

    char line_stick[64];
    char line_btns[80];
    char line_conn[48];

    while (1) {
        /* -------- UPDATE (input belongs here) -------- */

        /* Refresh hardware state into libdragon's snapshot. */
        joypad_poll();

        /* What kind of device is in port 1? NONE if unplugged. */
        joypad_style_t style = joypad_get_style(JOYPAD_PORT_1);

        /* Continuous state: stick axes + which buttons are down *right now*. */
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        /* Edges: buttons that went from up→down since last poll. */
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        int sx = apply_deadzone(in.stick_x);
        int sy = apply_deadzone(in.stick_y);

        if (style == JOYPAD_STYLE_NONE) {
            snprintf(line_conn, sizeof(line_conn), "Port 1: (no controller)");
        } else {
            snprintf(line_conn, sizeof(line_conn), "Port 1: connected");
        }

        snprintf(line_stick, sizeof(line_stick), "Stick: %+04d, %+04d", sx, sy);

        /* .a .b etc. are 0 or 1 for held state. */
        snprintf(line_btns, sizeof(line_btns),
                 "A:%d B:%d Z:%d Start:%d  L:%d R:%d",
                 in.btn.a, in.btn.b, in.btn.z, in.btn.start,
                 in.btn.l, in.btn.r);

        /* Edge demo: only true on the frame A is newly pressed. */
        const char *edge = pressed.a ? "A pressed this frame!" : "";

        /* -------- RENDER -------- */
        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 12, .g = 18, .b = 32, .a = 255 });

        rdpq_text_print(NULL, 1, 24, 40, "L03 — Controllers");
        rdpq_text_print(NULL, 1, 24, 70, line_conn);
        rdpq_text_print(NULL, 1, 24, 100, line_stick);
        rdpq_text_print(NULL, 1, 24, 120, line_btns);
        rdpq_text_print(NULL, 1, 24, 150, "D-pad / C / Z also on inputs.btn");
        if (edge[0]) {
            rdpq_text_print(NULL, 1, 24, 180, edge);
        }
        rdpq_text_print(NULL, 1, 24, 210, "Move stick, mash buttons (Ares: map input)");

        rdpq_detach_show();
    }
}
