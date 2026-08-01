/**
 * L09 — Meshes: vertices, triangles, winding, vertex color
 * ============================================================================
 * Mesh = triangles. Vertex = corner with attributes (here: pos + RGBA).
 * Colors blend across the triangle (Gouraud). A flips winding order.
 * Stick X rotates. RDP screen triangles stand in before Tiny3D meshes.
 * DOCS: docs/guide/m1/l09-meshes.md
 */


#include <libdragon.h>
#include <stdio.h>
#include "ng_math.h"

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    float rot = 0.f;
    bool flip_winding = false;
    char line[64];

    while (1) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        rot += (float)in.stick_x * 0.002f;
        if (pressed.a) {
            flip_winding = !flip_winding;
        }

        /* Local triangle */
        ng_vec3 p0 = ng_v3(0.f, -50.f, 0.f);
        ng_vec3 p1 = ng_v3(-55.f, 45.f, 0.f);
        ng_vec3 p2 = ng_v3(55.f, 45.f, 0.f);
        if (flip_winding) {
            ng_vec3 tmp = p1;
            p1 = p2;
            p2 = tmp;
        }

        ng_mat4 M;
        ng_mat4_trs_z(&M, ng_v3(160.f, 120.f, 0.f), rot, ng_v3(1.f, 1.f, 1.f));
        ng_vec3 w0 = ng_mat4_mul_point(&M, p0);
        ng_vec3 w1 = ng_mat4_mul_point(&M, p1);
        ng_vec3 w2 = ng_mat4_mul_point(&M, p2);

        /* TRIFMT_SHADE: X, Y, R, G, B, A */
        float v0[] = { w0.x, w0.y, 1.f, 0.2f, 0.2f, 1.f };
        float v1[] = { w1.x, w1.y, 0.2f, 1.f, 0.3f, 1.f };
        float v2[] = { w2.x, w2.y, 0.3f, 0.4f, 1.f, 1.f };

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 18, .g = 14, .b = 28, .a = 255 });

        rdpq_set_mode_standard();
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);
        rdpq_triangle(&TRIFMT_SHADE, v0, v1, v2);

        rdpq_text_print(NULL, 1, 12, 12, "L09 — Meshes & vertex color");
        snprintf(line, sizeof(line), "winding: %s  (A toggles)",
                 flip_winding ? "flipped" : "default");
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 200, "3 verts → 1 triangle; colors blend across (Gouraud)");
        rdpq_text_print(NULL, 1, 12, 216, "Stick X rotates. Mesh = many of these.");

        rdpq_detach_show();
    }
}
