/**
 * L06 — Space, points, vectors
 * ============================================================================
 * Point = location. Vector = direction + length (not a place by itself).
 * Stick builds a free vector from screen center.
 * C-right toggles adding a fixed "wind" vector (addition).
 * We print |v| (length) and normalized dir (length ~1).
 * DOCS: docs/guide/m1/l06-vectors.md
 */
\n

#include <libdragon.h>
#include <stdio.h>
#include "ng_math.h"

#define DEADZONE 8
#define SCALE    1.2f /* stick units → pixels */

static int dz(int v)
{
    return (v > -DEADZONE && v < DEADZONE) ? 0 : v;
}

static void draw_dot(float x, float y, color_t c)
{
    rdpq_set_mode_fill(c);
    rdpq_fill_rectangle((int)x - 2, (int)y - 2, (int)x + 3, (int)y + 3);
}

static void draw_line(float x0, float y0, float x1, float y1, color_t c)
{
    /* Cheap dotted line for teaching demos */
    float dx = x1 - x0;
    float dy = y1 - y0;
    float len = sqrtf(dx * dx + dy * dy);
    int steps = (int)(len / 3.f);
    if (steps < 1) {
        steps = 1;
    }
    rdpq_set_mode_fill(c);
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / (float)steps;
        int x = (int)(x0 + dx * t);
        int y = (int)(y0 + dy * t);
        rdpq_fill_rectangle(x, y, x + 2, y + 2);
    }
}

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    /* Fixed "wind" vector in screen space (pixels) — toggled contribution */
    const ng_vec2 wind = ng_v2(40.f, -15.f);
    bool add_wind = false;

    char line[72];

    while (1) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        if (pressed.c_right) {
            add_wind = !add_wind;
        }

        ng_vec2 stick = ng_v2((float)dz(in.stick_x) * SCALE,
                              (float)-dz(in.stick_y) * SCALE); /* screen +Y down */
        ng_vec2 v = stick;
        if (add_wind) {
            v = ng_v2_add(v, wind);
        }
        float len = ng_v2_len(v);
        ng_vec2 dir = ng_v2_normalize(v);

        float cx = 160.f;
        float cy = 120.f;

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 14, .g = 16, .b = 28, .a = 255 });

        /* Axes */
        draw_line(40, cy, 280, cy, (color_t){ 40, 40, 60, 255 });
        draw_line(cx, 40, cx, 200, (color_t){ 40, 40, 60, 255 });

        if (add_wind) {
            draw_line(cx, cy, cx + wind.x, cy + wind.y,
                      (color_t){ 80, 160, 255, 255 });
        }
        draw_line(cx, cy, cx + v.x, cy + v.y, (color_t){ 255, 200, 80, 255 });
        draw_dot(cx, cy, (color_t){ 220, 220, 220, 255 });
        draw_dot(cx + v.x, cy + v.y, (color_t){ 255, 100, 100, 255 });

        rdpq_text_print(NULL, 1, 12, 12, "L06 — Vectors");
        snprintf(line, sizeof(line), "v = (%+.0f, %+.0f)  |v|=%.1f", v.x, v.y, len);
        rdpq_text_print(NULL, 1, 12, 28, line);
        snprintf(line, sizeof(line), "dir = (%+.2f, %+.2f)", dir.x, dir.y);
        rdpq_text_print(NULL, 1, 12, 44, line);
        rdpq_text_print(NULL, 1, 12, 210,
                        add_wind ? "C-right: wind ON (v = stick + wind)"
                                 : "C-right: toggle wind addition");
        rdpq_text_print(NULL, 1, 12, 224, "Stick moves the free vector");

        rdpq_detach_show();
    }
}
