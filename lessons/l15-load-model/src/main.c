/**
 * L15 — Loading a glTF / Tiny3D model from the ROM
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Stop hand-packing every vertex. Load art from a file:
 *
 *   BUILD TIME:  assets/model.glb → gltf_to_t3d → filesystem/model.t3dm
 *                (+ textures as .sprite)
 *   RUN TIME:    dfs_init → t3d_model_load("rom:/model.t3dm")
 *                → record t3d_model_draw into an rspq block → run each frame
 *
 * MATRIX + DISPLAY LIST (important!)
 * ----------------------------------
 * When you *record* a model into an rspq block, match Tiny3D’s pattern:
 *
 *   1. Each frame:  t3d_matrix_push(&modelMat)
 *   2. Then:        rspq_block_run(dpl)   // block contains draw + matrix_pop
 *
 * The recorded block must end with t3d_matrix_pop(1) so the push is balanced.
 * Using only t3d_matrix_set before a recorded block (without push/pop) leaves
 * the model with no usable world matrix — you get clear color + HUD text but
 * an invisible mesh (the bug this lesson used to have).
 *
 * MATRIX BUFFERING
 * ----------------
 * One T3DMat4FP per framebuffer index; rotate the index each frame so the CPU
 * never overwrites a matrix the RSP still needs.
 *
 * DOCS: docs/guide/m2/l15-load-model.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>

#define FB_COUNT 3

/* Sample model uses prim-color on a band — animate it so you notice motion. */
static color_t rainbow_color(float s)
{
    float r = fm_sinf(s + 0.0f) * 127.0f + 128.0f;
    float g = fm_sinf(s + 2.0f) * 127.0f + 128.0f;
    float b = fm_sinf(s + 4.0f) * 127.0f + 128.0f;
    return RGBA32((int)r, (int)g, (int)b, 255);
}

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();
    asset_init_compression(2);

    dfs_init(DFS_DEFAULT_LOCATION);
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE,
                 FILTERS_RESAMPLE_ANTIALIAS);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    t3d_init((T3DInitParams){});

    /* One matrix per framebuffer so the RSP never reads a matrix mid-update. */
    T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);

    T3DModel *model = t3d_model_load("rom:/model.t3dm");
    if (!model) {
        while (1) {
            surface_t *disp = display_get();
            rdpq_attach(disp, NULL);
            rdpq_clear((color_t){ 80, 0, 0, 255 });
            rdpq_text_print(NULL, 1, 20, 100, "Failed rom:/model.t3dm");
            rdpq_detach_show();
        }
    }

    /*
     * Record once (lazy-style also fine). Must include matrix_pop so it pairs
     * with t3d_matrix_push before rspq_block_run — same as Tiny3D examples/01_model.
     */
    rspq_block_begin();
    t3d_model_draw(model);
    t3d_matrix_pop(1);
    rspq_block_t *dpl = rspq_block_end();

    const fm_vec3_t camPos = {{ 0, 10.0f, 40.0f }};
    const fm_vec3_t camTarget = {{ 0, 0, 0 }};
    uint8_t colorAmbient[4] = { 80, 80, 100, 0xFF };
    uint8_t colorDir[4] = { 0xEE, 0xAA, 0xAA, 0xFF };
    fm_vec3_t lightDir = {{ -1.0f, 1.0f, 1.0f }};
    fm_vec3_norm(&lightDir, &lightDir);

    float rot = 0.0f;
    int frameIdx = 0;

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        frameIdx = (frameIdx + 1) % FB_COUNT;
        rot += 0.015f + (float)in.stick_x * 0.0004f;

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 150.0f);
        t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(fm_vec3_t){{ 0, 1, 0 }});

        float modelScale = 0.1f;
        t3d_mat4fp_from_srt_euler(&modelMatFP[frameIdx],
            (float[3]){ modelScale, modelScale, modelScale },
            (float[3]){ 0.0f, rot * 0.2f, rot },
            (float[3]){ 0, 0, 0 });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        t3d_screen_clear_color(RGBA32(90, 70, 70, 0xFF));
        t3d_screen_clear_depth();

        t3d_light_set_ambient(colorAmbient);
        t3d_light_set_directional(0, colorDir, &lightDir);
        t3d_light_set_count(1);

        /* Dynamic prim color — sample mesh has a band that uses it. */
        rdpq_set_prim_color(rainbow_color(rot * 0.42f));

        /* World matrix, then run recorded draw+pop. */
        t3d_matrix_push(&modelMatFP[frameIdx]);
        rspq_block_run(dpl);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L15 — Load model (glTF → t3dm)");
        rdpq_text_print(NULL, 1, 12, 28, "rom:/model.t3dm  Stick X: spin speed");

        rdpq_detach_show();
    }
}
