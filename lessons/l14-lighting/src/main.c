/**
 * L14 — Lighting in Tiny3D
 * ============================================================================
 * Ambient = fill light everywhere (always on).
 * Directional = sun: direction + color; needs vertex normals.
 * Stick Y = ambient level; Stick X = orbit sun; A = toggle directional.
 * DOCS: docs/guide/m2/l14-lighting.md
 */


#include <libdragon.h>
#include <t3d/t3d.h>
#include <stdio.h>

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();
    t3d_init((T3DInitParams){});

    T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP));

    /* "Box" as 6 quads would be long — use one subdivided-ish fan cube via 8 corners.
     * Simpler: spinning sphere-ish is hard without model. Use a cube of 12 tris.
     * For teaching size, one double-sided lit quad + a second facing another way.
     */
    T3DVertPacked *vFront = malloc_uncached(sizeof(T3DVertPacked) * 2);
    T3DVertPacked *vTop = malloc_uncached(sizeof(T3DVertPacked) * 2);

    uint16_t nFront = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 0, 1 }});
    uint16_t nTop = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 1, 0 }});
    uint32_t white = 0xE0E0E0FF;

    vFront[0] = (T3DVertPacked){
        .posA = { -14, -14, 14 }, .rgbaA = white, .normA = nFront,
        .posB = { 14, -14, 14 },  .rgbaB = white, .normB = nFront,
    };
    vFront[1] = (T3DVertPacked){
        .posA = { 14, 14, 14 },  .rgbaA = white, .normA = nFront,
        .posB = { -14, 14, 14 }, .rgbaB = white, .normB = nFront,
    };
    vTop[0] = (T3DVertPacked){
        .posA = { -14, 14, -14 }, .rgbaA = white, .normA = nTop,
        .posB = { 14, 14, -14 },  .rgbaB = white, .normB = nTop,
    };
    vTop[1] = (T3DVertPacked){
        .posA = { 14, 14, 14 },  .rgbaA = white, .normA = nTop,
        .posB = { -14, 14, 14 }, .rgbaB = white, .normB = nTop,
    };

    float modelYaw = 0.f;
    float lightYaw = 0.6f;
    float ambientLevel = 0.25f;
    bool dirOn = true;
    char line[80];

    T3DViewport viewport = t3d_viewport_create();
    const fm_vec3_t camPos = {{ 28, 22, 36 }};
    const fm_vec3_t camTarget = {{ 0, 0, 0 }};

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        modelYaw += 0.012f;
        lightYaw += (float)in.stick_x * 0.0012f;
        ambientLevel += (float)in.stick_y * 0.0008f;
        if (ambientLevel < 0.0f) {
            ambientLevel = 0.0f;
        }
        if (ambientLevel > 0.9f) {
            ambientLevel = 0.9f;
        }
        if (pressed.a) {
            dirOn = !dirOn;
        }

        uint8_t amb = (uint8_t)(ambientLevel * 255.f);
        uint8_t colorAmbient[4] = { amb, amb, (uint8_t)(amb + 10 > 255 ? 255 : amb + 10), 0xFF };
        uint8_t colorDir[4] = { 0xFF, 0xE8, 0xC0, 0xFF };
        fm_vec3_t lightDir = {{
            fm_sinf(lightYaw),
            0.55f,
            fm_cosf(lightYaw),
        }};
        fm_vec3_norm(&lightDir, &lightDir);

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.0f), 5.0f, 120.0f);
        t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(fm_vec3_t){{ 0, 1, 0 }});

        t3d_mat4fp_from_srt_euler(modelMatFP,
            (float[3]){ 0.45f, 0.45f, 0.45f },
            (float[3]){ 0.2f, modelYaw, 0.1f },
            (float[3]){ 0, 0, 0 });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(12, 14, 22, 0));
        t3d_screen_clear_depth();

        /* Shade from vertex colors (+ lighting when SHADED). Without this, tris often draw black. */
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);

        t3d_light_set_ambient(colorAmbient);
        if (dirOn) {
            t3d_light_set_directional(0, colorDir, &lightDir);
            t3d_light_set_count(1);
        } else {
            t3d_light_set_count(0);
        }

        t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

        /* PUSH so the model composes with the camera matrix on the stack;
         * POP once the verts are loaded (t3d_matrix_set here would overwrite
         * the camera → blank mesh). */
        t3d_matrix_push(modelMatFP);
        t3d_vert_load(vFront, 0, 4);
        t3d_matrix_pop(1);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();
        t3d_matrix_push(modelMatFP);
        t3d_vert_load(vTop, 0, 4);
        t3d_matrix_pop(1);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L14 — Lighting");
        snprintf(line, sizeof(line), "ambient=%.2f  dir=%s  (A toggles sun)",
                 ambientLevel, dirOn ? "ON" : "OFF");
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 44, "Stick X: light orbit  Stick Y: ambient");

        rdpq_detach_show();
    }
}
