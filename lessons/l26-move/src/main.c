/**
 * L26 — Player movement in 3D
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Move a character on a flat-ish island using the N64 stick, with motion
 * relative to the *camera*, not raw world axes.
 *
 * WHY CAMERA-RELATIVE?
 * --------------------
 * If stick-up always meant "+Z world", it would feel broken whenever the
 * camera faces another way. Players expect stick-up = "into the screen."
 *
 * RECIPE
 * ------
 *   1. Read stick as (sx, sy) roughly in [-1, 1]
 *   2. Rotate that 2D vector by camera yaw into world XZ
 *   3. position += direction * speed * dt
 *   4. Face the player toward the movement direction (smooth yaw)
 *
 * CONTROLS: Stick = move
 * BUILD:    make -C lessons/l26-move
 * DOCS:     docs/guide/m4/l26-move.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <stdio.h>
#include "ng_game.h"

/** Triple-buffer color frames (common for 3D). */
#define FB_COUNT 3

int main(void)
{
    /* --- Standard libdragon + Tiny3D boot (see capstone for a longer tour) --- */
    debug_init_isviewer();
    debug_init_usblog();
    asset_init_compression(2);
    dfs_init(DFS_DEFAULT_LOCATION);
    /* FILTERS_RESAMPLE only — RESAMPLE_ANTIALIAS can leave a 1px flickering
     * edge line at the top of the framebuffer (VI AA / divot). */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    t3d_init((T3DInitParams){});

    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);

    /*
     * Uncached matrices: the RSP DMAs these. One player matrix per framebuffer
     * so we never overwrite a matrix the RSP is still reading.
     */
    T3DMat4FP *playerMat = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));

    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_static.t3dm"); /* no skinning yet */

    /* Island stays put forever. */
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 0.032f, 0.032f, 0.032f }, /* ~12u: t3dm island ±384 */
        (float[3]){ 0, 0, 0 },
        (float[3]){ 0, 0, 0 });

    /* Player pose in the world. y=0.15 keeps feet slightly above the mesh. */
    fm_vec3_t pos = {{ 0, 0.1f, 0 }};
    float yaw = 0.f; /* radians around +Y — which way the character faces */

    float last = ng_time_s();
    int frame = 0;
    char line[72];

    /* Simple daylight-ish lighting. */
    uint8_t amb[4] = { 80, 90, 100, 255 };
    uint8_t dirC[4] = { 255, 230, 200, 255 };
    fm_vec3_t ldir = {{ 0.3f, 1.f, 0.2f }};
    fm_vec3_norm(&ldir, &ldir);

    /*
     * Fixed camera yaw for this lesson (we do NOT orbit yet).
     * L28 will attach the camera behind the player instead.
     */
    float camYaw = 0.6f;

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        /* Seconds since last frame — makes speed frame-rate independent. */
        float now = ng_time_s();
        float dt = now - last;
        last = now;
        frame = (frame + 1) % FB_COUNT;

        /* ----- Stick as a 2D vector with deadzone (Module 0 L03) ----- */
        float sx = (float)ng_dz(in.stick_x) / 80.f;
        float sy = (float)ng_dz(in.stick_y) / 80.f;
        float speed = sqrtf(sx * sx + sy * sy);
        /* Clamp so diagonal isn't faster than cardinal directions. */
        if (speed > 1.f) {
            sx /= speed;
            sy /= speed;
            speed = 1.f;
        }

        /*
         * Rotate stick into world space using camera yaw.
         * Camera sits at offset (sin θ, cos θ) looking at the player, so look
         * direction on XZ is (-sin θ, -cos θ). Stick-up (positive sy) moves
         * that way (into the screen); stick-right moves camera-right.
         */
        float c = fm_cosf(camYaw);
        float s = fm_sinf(camYaw);
        float mx = sx * c - sy * s;
        float mz = -sx * s - sy * c;

        const float moveSpeed = 8.f; /* world units per second */
        if (speed > 0.15f) {
            pos.v[0] += mx * moveSpeed * dt;
            pos.v[2] += mz * moveSpeed * dt;

            /*
             * Face the direction of travel.
             * atan2f(mx, mz) gives a yaw; ng_lerp_angle smooths turn rate
             * so the character doesn't snap 180° instantly.
             */
            float targetYaw = atan2f(mx, mz);
            yaw = ng_lerp_angle(yaw, targetYaw, 0.2f);
        }

        /* Soft box bounds — island is small; keep the player visible. */
        pos.v[0] = ng_clamp(pos.v[0], -5.5f, 5.5f);
        pos.v[2] = ng_clamp(pos.v[2], -5.5f, 5.5f);

        /* ----- Camera: hard follow at a fixed offset (no lag yet) ----- */
        fm_vec3_t eye = {{
            pos.v[0] + fm_sinf(camYaw) * 12.f,
            pos.v[1] + 7.f,
            pos.v[2] + fm_cosf(camYaw) * 12.f,
        }};
        fm_vec3_t target = {{ pos.v[0], pos.v[1] + 1.f, pos.v[2] }};

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.f), 1.f, 200.f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Model matrix: scale, rotate by yaw, translate to pos (Module 1 L07).
         * player_static t3dm is tall in integer units (~y 13..107); ~0.04 ≈ 3–4u
         * on the ~12u island (0.08 felt oversized). */
        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.04f, 0.04f, 0.04f },
            (float[3]){ 0, yaw, 0 },
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        /* ----- Draw ----- */
        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(40, 70, 110, 0xFF));
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        /* PUSH/POP per object so each model composes with the camera matrix
         * that viewport_attach placed on the stack (matrix_set would overwrite
         * it → blank mesh). */
        t3d_matrix_push(islandMat);
        if (island) {
            t3d_model_draw(island);
        }
        t3d_matrix_pop(1);
        t3d_matrix_push(&playerMat[frame]);
        if (player) {
            t3d_model_draw(player);
        }
        t3d_matrix_pop(1);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L26 — Move (camera-relative)");
        snprintf(line, sizeof(line), "pos (%.1f, %.1f)  speed %.2f", pos.v[0], pos.v[2], speed);
        rdpq_text_print(NULL, 1, 10, 28, line);
        rdpq_text_print(NULL, 1, 10, 44, "Stick moves on island plane");

        rdpq_detach_show();
    }
}
