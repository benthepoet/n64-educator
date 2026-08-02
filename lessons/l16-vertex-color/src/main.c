/**
 * L16 — Vertex colors in Tiny3D
 * ============================================================================
 *
 * Terrain grid: each corner stores RGBA paint (grass / dirt variation).
 * A toggles painted verts vs flat green — same mesh, different attributes.
 * This is the N64 "texture × vertex color" mindset without a texture yet.
 *
 * DRAW NOTE (Tiny3D manual geometry)
 * ----------------------------------
 * T&L runs at t3d_vert_load time. Pattern from Tiny3D examples/00_quad:
 *   matrix_push → vert_load → matrix_pop → tri_draw → tri_sync
 *
 * DOCS: docs/guide/m2/l16-vertex-color.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>
#include <stdio.h>

#define GRID 6
#define QUADS ((GRID) * (GRID))
/* Two T3DVertPacked structs = 4 corners per quad */
#define PACKED_PER_QUAD 2
#define VERT_PACKED_COUNT (QUADS * PACKED_PER_QUAD)

/* t 0..1 → dirt brown to bright grass (0xRRGGBBAA) */
static uint32_t tint_rgba(float t)
{
    if (t < 0.f) {
        t = 0.f;
    }
    if (t > 1.f) {
        t = 1.f;
    }
    uint8_t r = (uint8_t)(60 + t * 40);
    uint8_t g = (uint8_t)(90 + t * 120);
    uint8_t b = (uint8_t)(40 + t * 30);
    return ((uint32_t)r << 24) | ((uint32_t)g << 16) | ((uint32_t)b << 8) | 0xFFu;
}

static float height_noise(int ix, int iz)
{
    return fm_sinf(ix * 0.9f) * 2.0f + fm_cosf(iz * 1.1f) * 1.5f;
}

static float paint_noise(int ix, int iz)
{
    float v = 0.55f + 0.35f * fm_sinf(ix * 0.8f + iz * 0.5f)
              + 0.15f * fm_cosf(iz * 1.3f);
    if (v < 0.f) {
        v = 0.f;
    }
    if (v > 1.f) {
        v = 1.f;
    }
    return v;
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

    T3DVertPacked *grid = malloc_uncached(sizeof(T3DVertPacked) * VERT_PACKED_COUNT);
    T3DVertPacked *gridFlat = malloc_uncached(sizeof(T3DVertPacked) * VERT_PACKED_COUNT);

    /* Horizontal ground plane: normal +Y so lighting reads as "floor". */
    uint16_t nUp = t3d_vert_pack_normal(&(fm_vec3_t){{ 0, 1, 0 }});

    /*
     * Model-space units similar to L11/L12 quads (tens of units).
     * World matrix will scale slightly; camera orbits at a comfortable distance.
     */
    const int cell = 12;
    const int origin = -((GRID * cell) / 2);

    int qi = 0;
    for (int iz = 0; iz < GRID; iz++) {
        for (int ix = 0; ix < GRID; ix++) {
            int16_t x0 = (int16_t)(origin + ix * cell);
            int16_t z0 = (int16_t)(origin + iz * cell);
            int16_t x1 = (int16_t)(x0 + cell);
            int16_t z1 = (int16_t)(z0 + cell);

            int16_t y00 = (int16_t)height_noise(ix, iz);
            int16_t y10 = (int16_t)height_noise(ix + 1, iz);
            int16_t y11 = (int16_t)height_noise(ix + 1, iz + 1);
            int16_t y01 = (int16_t)height_noise(ix, iz + 1);

            uint32_t c00 = tint_rgba(paint_noise(ix, iz));
            uint32_t c10 = tint_rgba(paint_noise(ix + 1, iz));
            uint32_t c11 = tint_rgba(paint_noise(ix + 1, iz + 1));
            uint32_t c01 = tint_rgba(paint_noise(ix, iz + 1));
            uint32_t flat = 0x70C070FF; /* mid green when paint is off */

            /*
             * Quad corners (XZ plane, Y up), CCW when viewed from +Y:
             *   0 (x0,z0) -- 1 (x1,z0)
             *   |                  |
             *   3 (x0,z1) -- 2 (x1,z1)
             * Tris: (0,1,2) and (2,3,0)
             */
            int base = qi * PACKED_PER_QUAD;
            grid[base + 0] = (T3DVertPacked){
                .posA = { x0, y00, z0 }, .rgbaA = c00, .normA = nUp,
                .posB = { x1, y10, z0 }, .rgbaB = c10, .normB = nUp,
            };
            grid[base + 1] = (T3DVertPacked){
                .posA = { x1, y11, z1 }, .rgbaA = c11, .normA = nUp,
                .posB = { x0, y01, z1 }, .rgbaB = c01, .normB = nUp,
            };
            gridFlat[base + 0] = (T3DVertPacked){
                .posA = { x0, y00, z0 }, .rgbaA = flat, .normA = nUp,
                .posB = { x1, y10, z0 }, .rgbaB = flat, .normB = nUp,
            };
            gridFlat[base + 1] = (T3DVertPacked){
                .posA = { x1, y11, z1 }, .rgbaA = flat, .normA = nUp,
                .posB = { x0, y01, z1 }, .rgbaB = flat, .normB = nUp,
            };
            qi++;
        }
    }

    /* One model matrix (identity-ish scale); content updated if we ever animate. */
    T3DMat4FP *worldMat = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_from_srt_euler(worldMat,
        (float[3]){ 0.55f, 0.55f, 0.55f },
        (float[3]){ 0, 0, 0 },
        (float[3]){ 0, 0, 0 });

    float yaw = 0.6f;
    float pitch = 0.55f;
    float dist = 55.f;
    bool showPaint = true;
    char line[72];

    T3DViewport viewport = t3d_viewport_create();
    uint8_t ambient[4] = { 90, 90, 100, 0xFF };
    uint8_t dirCol[4] = { 0xFF, 0xF0, 0xD0, 0xFF };
    fm_vec3_t lightDir = {{ 0.35f, 1.0f, 0.25f }};
    fm_vec3_norm(&lightDir, &lightDir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        if (joypad_get_buttons_pressed(JOYPAD_PORT_1).a) {
            showPaint = !showPaint;
        }

        yaw += (float)in.stick_x * 0.0012f;
        pitch += (float)in.stick_y * 0.0010f;
        if (pitch < 0.20f) {
            pitch = 0.20f;
        }
        if (pitch > 1.25f) {
            pitch = 1.25f;
        }

        fm_vec3_t target = {{ 0, 0, 0 }};
        fm_vec3_t eye = {{
            dist * fm_cosf(pitch) * fm_sinf(yaw),
            dist * fm_sinf(pitch) + 6.f,
            dist * fm_cosf(pitch) * fm_cosf(yaw),
        }};

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(65.0f), 5.0f, 200.0f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(30, 50, 80, 0xFF));
        t3d_screen_clear_depth();

        /* Shade from vertex colors (+ lighting when SHADED). Without this, tris often draw black. */
        rdpq_mode_combiner(RDPQ_COMBINER_SHADE);

        t3d_light_set_ambient(ambient);
        t3d_light_set_directional(0, dirCol, &lightDir);
        t3d_light_set_count(1);
        t3d_state_set_drawflags(T3D_FLAG_SHADED | T3D_FLAG_DEPTH);

        T3DVertPacked *use = showPaint ? grid : gridFlat;

        /*
         * Load + draw each quad. Matrix push/pop around vert_load so T&L uses
         * the model matrix; triangles use the transformed cache (00_quad style).
         */
        for (int q = 0; q < QUADS; q++) {
            t3d_matrix_push(worldMat);
            t3d_vert_load(&use[q * PACKED_PER_QUAD], 0, 4);
            t3d_matrix_pop(1);
            t3d_tri_draw(0, 1, 2);
            t3d_tri_draw(2, 3, 0);
            t3d_tri_sync();
        }

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 12, 12, "L16 — Vertex color terrain");
        snprintf(line, sizeof(line), "paint %s  (A toggles)", showPaint ? "ON" : "OFF");
        rdpq_text_print(NULL, 1, 12, 28, line);
        rdpq_text_print(NULL, 1, 12, 44, "Stick orbit — same mesh, different vert RGBA");

        rdpq_detach_show();
    }
}
