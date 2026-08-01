/**
 * L13 — Orbit camera in 3D
 * ============================================================================
 * Place the camera on a sphere around a target:
 *   eye = (dist * cos(pitch) * sin(yaw), dist * sin(pitch), dist * cos(pitch) * cos(yaw))
 * Stick changes yaw/pitch; C-up/dn changes dist; look_at builds the view matrix.
 * DOCS: docs/guide/m2/l13-orbit-camera.md
 */


#include <libdragon.h>
#include <t3d/t3d.h>

#define DEADZONE 8

static int dz(int v)
{
    return (v > -DEADZONE && v < DEADZONE) ? 0 : v;
}

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
    T3DVertPacked *verts = malloc_uncached(sizeof(T3DVertPacked) * 2);
    uint16_t norm = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 1, 0 }});
    /* Flat ground-ish quad in XZ (Y up). */
    verts[0] = (T3DVertPacked){
        .posA = { -20, 0, -20 }, .rgbaA = 0x60A060FF, .normA = norm,
        .posB = { 20, 0, -20 },  .rgbaB = 0x60A060FF, .normB = norm,
    };
    verts[1] = (T3DVertPacked){
        .posA = { 20, 0, 20 },  .rgbaA = 0x408040FF, .normA = norm,
        .posB = { -20, 0, 20 }, .rgbaB = 0x408040FF, .normB = norm,
    };

    T3DMat4FP *markerMat = malloc_uncached(sizeof(T3DMat4FP));
    T3DVertPacked *marker = malloc_uncached(sizeof(T3DVertPacked) * 2);
    uint16_t n2 = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 0, 1 }});
    marker[0] = (T3DVertPacked){
        .posA = { -3, -3, 0 }, .rgbaA = 0xFF4040FF, .normA = n2,
        .posB = { 3, -3, 0 },  .rgbaB = 0xFF4040FF, .normB = n2,
    };
    marker[1] = (T3DVertPacked){
        .posA = { 3, 3, 0 },  .rgbaA = 0xFF4040FF, .normA = n2,
        .posB = { -3, 3, 0 }, .rgbaB = 0xFF4040FF, .normB = n2,
    };

    float yaw = 0.6f;
    float pitch = 0.4f;
    float dist = 45.f;
    char line[72];

    T3DViewport viewport = t3d_viewport_create();
    uint8_t ambient[4] = { 50, 50, 60, 0xFF };
    uint8_t dirCol[4] = { 0xEE, 0xEE, 0xCC, 0xFF };
    fm_vec3_t lightDir = {{ 0.4f, 1.0f, 0.3f }};
    fm_vec3_norm(&lightDir, &lightDir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        yaw += (float)dz(in.stick_x) * 0.0015f;
        pitch += (float)dz(in.stick_y) * 0.0012f;
        if (pitch < 0.05f) {
            pitch = 0.05f;
        }
        if (pitch > 1.4f) {
            pitch = 1.4f;
        }
        if (in.btn.c_up) {
            dist -= 0.35f;
        }
        if (in.btn.c_down) {
            dist += 0.35f;
        }
        if (dist < 15.f) {
            dist = 15.f;
        }
        if (dist > 90.f) {
            dist = 90.f;
        }

        /* Spherical orbit around origin target. */
        fm_vec3_t target = {{ 0, 0, 0 }};
        fm_vec3_t eye = {{
            dist * fm_cosf(pitch) * fm_sinf(yaw),
            dist * fm_sinf(pitch),
            dist * fm_cosf(pitch) * fm_cosf(yaw),
        }};

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(65.0f), 5.0f, 150.0f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        t3d_mat4fp_from_srt_euler(modelMatFP,
            (float[3]){ 1, 1, 1 },
            (float[3]){ 0, 0, 0 },
            (float[3]){ 0, 0, 0 });
        t3d_mat4fp_from_srt_euler(markerMat,
            (float[3]){ 0.4f, 0.4f, 0.4f },
            (float[3]){ 0, yaw, 0 },
            (float[3]){ 0, 4, 0 });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(25, 35, 55, 0));
        t3d_screen_clear_depth();

        /* Shade from vertex colors (+ lighting when SHADED). Without this, tris often draw black. */
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);

        t3d_light_set_ambient(ambient);
        t3d_light_set_directional(0, dirCol, &lightDir);
        t3d_light_set_count(1);
        t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

        t3d_matrix_set(modelMatFP, true);
        t3d_vert_load(verts, 0, 4);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();

        t3d_matrix_set(markerMat, true);
        t3d_vert_load(marker, 0, 4);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L13 — Orbit camera");
        snprintf(line, sizeof(line), "yaw=%.2f pitch=%.2f dist=%.0f", yaw, pitch, dist);
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 44, "Stick orbit  C-up/dn dolly");

        rdpq_detach_show();
    }
}
