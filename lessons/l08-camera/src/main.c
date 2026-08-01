/**
 * L08 — Camera & projection (2D teaching analogue)
 * ============================================================================
 * Museum story: props stay put; you move the camera; props appear to slide.
 * screen = (world - camera) * zoom + center
 * Stick moves camera; A toggles zoom (lens intuition).
 * DOCS: docs/guide/m1/l08-camera.md
 */
\n

#include <libdragon.h>
#include <stdio.h>
#include "ng_math.h"

#define DEADZONE 8
#define NUM_PROPS 8

static int dz(int v)
{
    return (v > -DEADZONE && v < DEADZONE) ? 0 : v;
}

static void draw_dot(float x, float y, color_t c, int r)
{
    rdpq_set_mode_fill(c);
    rdpq_fill_rectangle((int)x - r, (int)y - r, (int)x + r + 1, (int)y + r + 1);
}

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    /* World-space prop positions */
    ng_vec2 props[NUM_PROPS] = {
        ng_v2(0, 0),     ng_v2(80, 0),   ng_v2(-70, 40), ng_v2(40, -60),
        ng_v2(-100, -30), ng_v2(120, 50), ng_v2(20, 90),  ng_v2(-40, -100),
    };

    ng_vec2 cam = ng_v2(0, 0);
    float zoom = 1.f;
    char line[72];

    while (1) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        cam.x += (float)dz(in.stick_x) * 0.2f;
        cam.y -= (float)dz(in.stick_y) * 0.2f;
        if (pressed.a) {
            zoom = (zoom < 1.5f) ? 1.8f : 1.f;
        }

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 10, .g = 14, .b = 24, .a = 255 });

        float cx = 160.f, cy = 120.f;

        /* View: screen = (world - cam) * zoom + screen_center */
        for (int i = 0; i < NUM_PROPS; i++) {
            ng_vec2 w = props[i];
            float sx = (w.x - cam.x) * zoom + cx;
            float sy = (w.y - cam.y) * zoom + cy;
            if (sx < -10 || sx > 330 || sy < -10 || sy > 250) {
                continue;
            }
            color_t c = (i == 0) ? (color_t){ 255, 80, 80, 255 }
                                 : (color_t){ 100, 200, 255, 255 };
            draw_dot(sx, sy, c, (i == 0) ? 4 : 3);
        }

        /* Camera reticle at screen center */
        draw_dot(cx, cy, (color_t){ 255, 255, 255, 255 }, 2);

        rdpq_text_print(NULL, 1, 12, 12, "L08 — Camera (2D view)");
        snprintf(line, sizeof(line), "cam=(%+.0f,%+.0f)  zoom=%.1f", cam.x, cam.y, zoom);
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 200, "Stick moves camera  A toggles zoom");
        rdpq_text_print(NULL, 1, 12, 216, "Red prop is world origin. Screen = (world-cam)*zoom");

        rdpq_detach_show();
    }
}
