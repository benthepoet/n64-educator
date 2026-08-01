/**
 * L11 — Tiny3D first light: a spinning colored quad
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * First real 3D pixels via Tiny3D. Map Module 1 words onto API calls:
 *
 *   Module 1 idea          | Tiny3D / this file
 *   -----------------------|------------------------------------------
 *   Projection (lens)      | t3d_viewport_set_projection
 *   View (camera)          | t3d_viewport_look_at
 *   Model matrix           | fm_mat4_* then t3d_mat4_to_fixed
 *   Vertices + colors      | T3DVertPacked (2 verts per struct)
 *   Ambient + directional  | t3d_light_set_*
 *   Depth buffer           | display_get_zbuf + T3D_FLAG_DEPTH
 *
 * FRAME RECIPE (memorize)
 * -----------------------
 *   update matrices
 *   rdpq_attach(color, depth)
 *   t3d_frame_start()
 *   t3d_viewport_attach()
 *   clear color + clear depth
 *   set lights + draw flags
 *   draw geometry
 *   optional 2D text
 *   rdpq_detach_show()
 *
 * BUILD: make -C lessons/l11-t3d-quad
 * DOCS:  docs/guide/m2/l11-t3d-quad.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();

    /* 3 framebuffers: CPU can prepare the next frame while RDP finishes one. */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    /* Boot Tiny3D (RSP ucode + C helpers). Empty struct = library defaults. */
    t3d_init((T3DInitParams){});

    /*
     * WHY malloc_uncached?
     * The RSP DMAs matrices/verts from RDRAM. CPU cache can hide recent writes
     * from DMA. Uncached pointers avoid "I rotated the matrix but nothing moved."
     */
    T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP));

    /* Camera on -Z looking at origin; +Y is up (course convention). */
    const fm_vec3_t camPos = {{ 0, 0, -18 }};
    const fm_vec3_t camTarget = {{ 0, 0, 0 }};

    /* Soft fill light so unlit sides aren't pure black + one white "sun". */
    uint8_t colorAmbient[4] = { 50, 50, 50, 0xFF };
    uint8_t colorDir[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
    fm_vec3_t lightDir = {{ 0.0f, 0.0f, 1.0f }};
    fm_vec3_norm(&lightDir, &lightDir);

    /*
     * T3DVertPacked stores TWO vertices (A and B) for efficient DMA.
     * We need 4 corners → 2 packed structs.
     *   pos*  = model-space position
     *   rgba* = 0xRRGGBBAA
     *   norm* = packed normal (lighting needs a facing direction)
     */
    T3DVertPacked *vertices = malloc_uncached(sizeof(T3DVertPacked) * 2);
    uint16_t norm = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 0, 1 }}); /* face +Z */
    vertices[0] = (T3DVertPacked){
        .posA = { -16, -16, 0 }, .rgbaA = 0xFF0000FF, .normA = norm, /* red */
        .posB = { 16, -16, 0 },  .rgbaB = 0x00FF00FF, .normB = norm, /* green */
    };
    vertices[1] = (T3DVertPacked){
        .posA = { 16, 16, 0 },  .rgbaA = 0x0000FFFF, .normA = norm, /* blue */
        .posB = { -16, 16, 0 }, .rgbaB = 0xFF00FFFF, .normB = norm, /* magenta */
    };

    float rotAngle = 0.0f;
    fm_vec3_t rotAxis = {{ -1.0f, 2.5f, 0.25f }};
    fm_vec3_norm(&rotAxis, &rotAxis);

    /* Viewport = projection + view matrices + scissor for the whole screen. */
    T3DViewport viewport = t3d_viewport_create();

    /*
     * Display list: we *record* the draw commands once, then replay each frame.
     * Updating modelMatFP still spins the quad because the matrix is DMA'd
     * from that pointer every time the list runs.
     */
    rspq_block_t *dplDraw = NULL;
    fm_mat4_t modelMat; /* float matrix we convert to fixed-point for the RSP */

    for (;;) {
        /* ===== UPDATE ===== */
        rotAngle += 0.03f;

        /* Lens: 85° FOV, near 10, far 100 (tune if the quad clips away). */
        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 100.0f);
        t3d_viewport_look_at(&viewport, &camPos, &camTarget, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Build a float model matrix, then convert for the RSP. */
        fm_mat4_identity(&modelMat);
        fm_mat4_from_axis_angle(&modelMat, &rotAxis, rotAngle);
        fm_mat4_scale(&modelMat, &(fm_vec3_t){{ 0.4f, 0.4f, 0.4f }});
        t3d_mat4_to_fixed(modelMatFP, &modelMat);

        /* ===== RENDER ===== */
        rdpq_attach(display_get(), display_get_zbuf()); /* color + depth */
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        t3d_screen_clear_color(RGBA32(40, 20, 60, 0));
        t3d_screen_clear_depth(); /* always clear Z or you get ghosts */

        t3d_light_set_ambient(colorAmbient);
        t3d_light_set_directional(0, colorDir, &lightDir);
        t3d_light_set_count(1);

        /* SHADED = use vertex colors + lights; DEPTH = Z-test/write. */
        t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

        if (!dplDraw) {
            rspq_block_begin();
            t3d_matrix_push(modelMatFP);   /* current model transform */
            t3d_vert_load(vertices, 0, 4); /* upload 4 verts */
            t3d_matrix_pop(1);
            /* Two triangles make a quad: (0,1,2) and (2,3,0). */
            t3d_tri_draw(0, 1, 2);
            t3d_tri_draw(2, 3, 0);
            t3d_tri_sync(); /* needed after a manual tri batch */
            dplDraw = rspq_block_end();
        }

        rspq_block_run(dplDraw); /* replay recorded draws */

        /* 2D overlay on the same frame (after 3D). */
        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L11 — Tiny3D quad");
        rdpq_text_print(NULL, 1, 12, 28, "4 verts, 2 tris, lit + depth");

        rdpq_detach_show();
    }
}
