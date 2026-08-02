/**
 * L27 — Drive skeletal animation from gameplay
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Don't pick Idle/Walk with a button. Let *movement speed* choose the blend:
 *   standing still → idle
 *   pushing stick  → walk
 *
 * PIPELINE (each frame)
 * ---------------------
 *   1. Move the player (camera-relative stick)
 *   2. t3d_anim_update(idle)   → pose into skeleton A
 *   3. t3d_anim_update(walk)   → pose into skeleton B
 *   4. t3d_skeleton_blend(A, B, blend)
 *   5. t3d_skeleton_update     → bone matrices (advances buffer index)
 *   6. t3d_skeleton_use        → REQUIRED for buffered skeletons before draw
 *   7. t3d_matrix_push + t3d_model_draw_skinned + pop
 *
 * Assets: Quaternius Easy Enemy (CC0) via Tiny3D examples/08_animation.
 * Clip names Snake_Idle / Snake_Walk come from that glTF.
 *
 * CONTROLS: Stick = move
 * BUILD:    make -C lessons/l27-anim-drive
 * DOCS:     docs/guide/m4/l27-anim-drive.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <stdio.h>
#include "ng_game.h"

#define FB_COUNT 3

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();
    asset_init_compression(2);

    dfs_init(DFS_DEFAULT_LOCATION);
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    t3d_init((T3DInitParams){});

    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);

    T3DMat4FP *playerMat = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DMat4FP *mapMat = malloc_uncached(sizeof(T3DMat4FP));
    /*
     * Map scale / placement matches Tiny3D examples/08_animation — these meshes
     * are authored in large integer units; tiny scales keep them on screen.
     */
    t3d_mat4fp_from_srt_euler(mapMat,
        (float[3]){ 0.3f, 0.3f, 0.3f },
        (float[3]){ 0, 0, 0 },
        (float[3]){ 0, 0, -10 });

    T3DModel *map = t3d_model_load("rom:/map.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");
    assertf(map && player, "Failed to load map/player models from ROM (DFS)");

    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);

    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);
    t3d_anim_attach(&animWalk, &skelBlend);

    /* Player sits slightly above the map surface used by the Tiny3D sample. */
    fm_vec3_t pos = {{ 0, 0.15f, -10 }};
    float yaw = 0.f;
    float camYaw = 0.0f;
    float last = ng_time_s();
    int frame = 0;
    char line[72];

    uint8_t amb[4] = { 0xAA, 0xAA, 0xAA, 0xFF };
    uint8_t dirC[4] = { 0xFF, 0xAA, 0xAA, 0xFF };
    fm_vec3_t ldir = {{ 1.f, 1.f, 1.f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        float now = ng_time_s();
        float dt = now - last;
        last = now;
        if (dt < 0.f) {
            dt = 0.f;
        }
        if (dt > 0.1f) {
            dt = 0.1f;
        }
        frame = (frame + 1) % FB_COUNT;

        /* Stick → camera-relative XZ move */
        float sx = (float)ng_dz(in.stick_x) / 80.f;
        float sy = (float)ng_dz(in.stick_y) / 80.f;
        float speed = sqrtf(sx * sx + sy * sy);
        if (speed > 1.f) {
            sx /= speed;
            sy /= speed;
            speed = 1.f;
        }

        /* Camera-relative: stick-up = into screen (along camera look on XZ). */
        float c = fm_cosf(camYaw), s = fm_sinf(camYaw);
        float mx = sx * c - sy * s;
        float mz = -sx * s - sy * c;

        float blend = 0.f;
        if (speed > 0.15f) {
            pos.v[0] += mx * 12.f * dt;
            pos.v[2] += mz * 12.f * dt;
            yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.25f);
            blend = ng_clamp(speed, 0.f, 1.f);
        }
        /* Soft bounds around the sample map */
        pos.v[0] = ng_clamp(pos.v[0], -40.f, 40.f);
        pos.v[2] = ng_clamp(pos.v[2], -50.f, 20.f);

        /* --- Animation --- */
        t3d_anim_update(&animIdle, dt);
        t3d_anim_set_speed(&animWalk, 0.15f + blend);
        t3d_anim_update(&animWalk, dt);
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        /* --- Camera (follow, elevated) --- */
        fm_vec3_t target = {{ pos.v[0], pos.v[1] + 6.f, pos.v[2] }};
        fm_vec3_t eye = {{
            pos.v[0] + fm_sinf(camYaw) * 55.f,
            pos.v[1] + 40.f,
            pos.v[2] + fm_cosf(camYaw) * 55.f,
        }};
        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(85.f), 10.f, 150.f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Character is authored large — scale matches Tiny3D 08_animation. */
        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.125f, 0.125f, 0.125f },
            (float[3]){ 0.f, -yaw, 0.f },
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        /* --- Draw --- */
        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF)); /* sample sand sky */
        t3d_screen_clear_depth();

        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_push(mapMat);
        t3d_model_draw(map);
        t3d_matrix_pop(1);

        t3d_skeleton_use(&skel);
        t3d_matrix_push(&playerMat[frame]);
        t3d_model_draw_skinned(player, &skel);
        t3d_matrix_pop(1);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L27 — Anim from speed");
        snprintf(line, sizeof(line), "blend=%.2f (0=idle 1=walk)", blend);
        rdpq_text_print(NULL, 1, 10, 28, line);
        rdpq_text_print(NULL, 1, 10, 44, "Stop = idle, push stick = walk blend");

        rdpq_detach_show();
    }
}
