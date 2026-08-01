/**
 * L12 — Depth buffer + multiple objects
 * ============================================================================
 * Each object = geometry + its own model matrix.
 * Depth (Z) buffer: nearer pixels win so draw order matters less.
 * A toggles T3D_FLAG_DEPTH — watch red/cyan fight when OFF.
 * DOCS: docs/guide/m2/l12-depth-objects.md
 */
\n

#include <libdragon.h>
#include <t3d/t3d.h>

static void make_quad(T3DVertPacked *verts, uint32_t rgba)
{
    uint16_t norm = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 0, 1 }});
    verts[0] = (T3DVertPacked){
        .posA = { -12, -12, 0 }, .rgbaA = rgba, .normA = norm,
        .posB = { 12, -12, 0 },  .rgbaB = rgba, .normB = norm,
    };
    verts[1] = (T3DVertPacked){
        .posA = { 12, 12, 0 },  .rgbaA = rgba, .normA = norm,
        .posB = { -12, 12, 0 }, .rgbaB = rgba, .normB = norm,
    };
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

    T3DMat4FP *matNear = malloc_uncached(sizeof(T3DMat4FP));
    T3DMat4FP *matFar = malloc_uncached(sizeof(T3DMat4FP));

    T3DVertPacked *quadRed = malloc_uncached(sizeof(T3DVertPacked) * 2);
    T3DVertPacked *quadCyan = malloc_uncached(sizeof(T3DVertPacked) * 2);
    make_quad(quadRed, 0xFF4040FF);
    make_quad(quadCyan, 0x40FFFFFF);

    const fm_vec3_t camPos = {{ 0, 0, -40 }};
    const fm_vec3_t camTarget = {{ 0, 0, 0 }};
    uint8_t ambient[4] = { 40, 40, 50, 0xFF };
    uint8_t dirCol[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    fm_vec3_t lightDir = {{ 0.2f, 0.4f, 1.0f }};
    fm_vec3_norm(&lightDir, &lightDir);

    T3DViewport viewport = t3d_viewport_create();
    bool useDepth = true;
    float t = 0.f;

    for (;;) {
        joypad_poll();
        if (joypad_get_buttons_pressed(JOYPAD_PORT_1).a) {
            useDepth = !useDepth;
        }
        t += 0.02f;

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(70.0f), 5.0f, 120.0f);
        t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Near red quad drifts left/right; far cyan sits behind and larger path. */
        t3d_mat4fp_from_srt_euler(matNear,
            (float[3]){ 0.35f, 0.35f, 0.35f },
            (float[3]){ 0, t * 0.4f, 0 },
            (float[3]){ fm_sinf(t) * 8.f, 0, 0 });
        t3d_mat4fp_from_srt_euler(matFar,
            (float[3]){ 0.55f, 0.55f, 0.55f },
            (float[3]){ 0, -t * 0.25f, 0 },
            (float[3]){ 0, 0, 14.f });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        t3d_screen_clear_color(RGBA32(20, 24, 40, 0));
        t3d_screen_clear_depth();

        t3d_light_set_ambient(ambient);
        t3d_light_set_directional(0, dirCol, &lightDir);
        t3d_light_set_count(1);

        uint16_t flags = T3D_FLAG_SHADED;
        if (useDepth) {
            flags |= T3D_FLAG_DEPTH;
        }
        t3d_state_set_drawflags(flags);

        /* Draw FAR first, then NEAR — with depth, order should not matter. */
        t3d_matrix_set(matFar, true);
        t3d_vert_load(quadCyan, 0, 4);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();

        t3d_matrix_set(matNear, true);
        t3d_vert_load(quadRed, 0, 4);
        t3d_tri_draw(0, 1, 2);
        t3d_tri_draw(2, 3, 0);
        t3d_tri_sync();

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L12 — Depth + two objects");
        rdpq_text_print(NULL, 1, 12, 28,
                        useDepth ? "Depth ON (A toggles)" : "Depth OFF — draw order wins");
        rdpq_text_print(NULL, 1, 12, 44, "Red=near  Cyan=far");

        rdpq_detach_show();
    }
}
