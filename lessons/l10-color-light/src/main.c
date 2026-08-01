/**
 * L10 — Color, lighting concepts, vertex-color blending
 * ============================================================================
 * Left triangle: flat base * ambient.
 * Right triangle: base * per-corner tints * ambient  ← N64 terrain idea.
 * Stick Y changes ambient. Same "material", spatial paint free of huge textures.
 * DOCS: docs/guide/m1/l10-color-light.md
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

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    char line[72];

    while (1) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        /* Map stick Y to ambient 0.25..1.0 */
        float ambient = 0.65f + (float)dz(in.stick_y) / 160.f;
        if (ambient < 0.25f) {
            ambient = 0.25f;
        }
        if (ambient > 1.f) {
            ambient = 1.f;
        }

        /* Base "albedo" green (like a grass texture sample) */
        float br = 0.25f, bg = 0.75f, bb = 0.30f;

        /* Left: flat shade of albedo * ambient (no spatial vertex variation) */
        float L0[] = { 40, 70, br * ambient, bg * ambient, bb * ambient, 1 };
        float L1[] = { 140, 70, br * ambient, bg * ambient, bb * ambient, 1 };
        float L2[] = { 90, 170, br * ambient, bg * ambient, bb * ambient, 1 };

        /* Right: vertex colors multiply albedo (path / dirt / bright spots) */
        float tint0 = 1.0f, tint1 = 0.45f, tint2 = 0.75f; /* per-vertex grayscale tints */
        float R0[] = { 180, 70, br * ambient * tint0, bg * ambient * tint0, bb * ambient * tint0, 1 };
        float R1[] = { 280, 70, br * ambient * tint1, bg * ambient * tint1, bb * ambient * tint1, 1 };
        float R2[] = { 230, 170, br * ambient * tint2, bg * ambient * tint2, bb * ambient * tint2, 1 };

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 16, .g = 16, .b = 22, .a = 255 });

        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
        rdpq_triangle(&TRIFMT_SHADE, L0, L1, L2);
        rdpq_triangle(&TRIFMT_SHADE, R0, R1, R2);

        rdpq_text_print(NULL, 1, 12, 12, "L10 — Lighting & vertex color blend");
        rdpq_text_print(NULL, 1, 40, 50, "Flat * ambient");
        rdpq_text_print(NULL, 1, 175, 50, "Albedo * vertex * amb");
        snprintf(line, sizeof(line), "ambient=%.2f  (stick Y)", ambient);
        rdpq_text_print(NULL, 1, 12, 190, line);
        rdpq_text_print(NULL, 1, 12, 210, "Right: one material, color varies by vertex (N64 classic)");
        rdpq_text_print(NULL, 1, 12, 224, "Next: Tiny3D real meshes, lights, textures");

        rdpq_detach_show();
    }
}
