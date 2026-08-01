/**
 * L07 — Matrices & transforms
 * ============================================================================
 * Local square corners → model matrix M = T * R * S → world corners.
 * Stick translates, L/R rotate, C-up/dn scale.
 * White dot = object origin. Order matters: scale, then rotate, then translate.
 * DOCS: docs/guide/m1/l07-matrices.md
 */
\n

#include <libdragon.h>
#include <stdio.h>
#include "ng_math.h"

#define DEADZONE 8

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
    float dx = x1 - x0, dy = y1 - y0;
    float len = sqrtf(dx * dx + dy * dy);
    int steps = (int)(len / 2.5f);
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

    float tx = 160.f, ty = 120.f;
    float rot = 0.f;
    float scale = 40.f;
    char line[80];

    /* Local-space corners of a square centered at origin */
    const ng_vec3 local[4] = {
        ng_v3(-0.5f, -0.5f, 0.f),
        ng_v3(0.5f, -0.5f, 0.f),
        ng_v3(0.5f, 0.5f, 0.f),
        ng_v3(-0.5f, 0.5f, 0.f),
    };

    while (1) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        tx += (float)dz(in.stick_x) * 0.15f;
        ty -= (float)dz(in.stick_y) * 0.15f;
        if (in.btn.l) {
            rot -= 0.04f;
        }
        if (in.btn.r) {
            rot += 0.04f;
        }
        if (in.btn.c_up) {
            scale += 0.5f;
        }
        if (in.btn.c_down) {
            scale -= 0.5f;
        }
        if (scale < 10.f) {
            scale = 10.f;
        }
        if (scale > 90.f) {
            scale = 90.f;
        }

        ng_mat4 M;
        ng_mat4_trs_z(&M, ng_v3(tx, ty, 0.f), rot, ng_v3(scale, scale, 1.f));

        ng_vec3 world[4];
        for (int i = 0; i < 4; i++) {
            world[i] = ng_mat4_mul_point(&M, local[i]);
        }

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 12, .g = 18, .b = 22, .a = 255 });

        color_t edge = { 100, 220, 160, 255 };
        for (int i = 0; i < 4; i++) {
            int j = (i + 1) % 4;
            draw_line(world[i].x, world[i].y, world[j].x, world[j].y, edge);
            draw_dot(world[i].x, world[i].y, (color_t){ 255, 220, 80, 255 });
        }
        draw_dot(tx, ty, (color_t){ 255, 255, 255, 255 });

        rdpq_text_print(NULL, 1, 12, 12, "L07 — Matrices (T * R * S)");
        snprintf(line, sizeof(line), "T=(%.0f,%.0f)  rot=%.0f deg  S=%.0f",
                 tx, ty, ng_rad_to_deg(rot), scale);
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 210, "Stick: translate  L/R: rotate  C-up/dn: scale");

        rdpq_detach_show();
    }
}
