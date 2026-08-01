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
 *   1. Move the player like L26 (get a 0..1 "speed" / blend factor)
 *   2. t3d_anim_update(idle)   → writes pose into skeleton A
 *   3. t3d_anim_update(walk)   → writes pose into skeleton B
 *   4. t3d_skeleton_blend(A, B, blend)
 *   5. t3d_skeleton_update     → bone matrices ready
 *   6. t3d_model_draw_skinned  → draw using that pose
 *
 * Clip names "Snake_Idle" / "Snake_Walk" come from the sample model's glTF.
 * Your own character will use whatever action names you exported (Module 3).
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
                 FILTERS_RESAMPLE_ANTIALIAS);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    t3d_init((T3DInitParams){});

    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);
    T3DMat4FP *playerMat = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 1, 1, 1 }, (float[3]){ 0, 0, 0 }, (float[3]){ 0, 0, 0 });

    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");

    /*
     * Skeleton = a list of bones + matrices describing the current pose.
     * We keep two: skel (what we draw) and skelBlend (walk pose only).
     */
    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);

    /* Named clips baked into the model file at export time. */
    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);      /* idle targets main skeleton */
    t3d_anim_attach(&animWalk, &skelBlend); /* walk targets blend skeleton */

    fm_vec3_t pos = {{ 0, 0.15f, 0 }};
    float yaw = 0.f;
    float camYaw = 0.6f;
    float last = ng_time_s();
    int frame = 0;
    char line[72];

    uint8_t amb[4] = { 90, 90, 110, 255 };
    uint8_t dirC[4] = { 255, 200, 200, 255 };
    fm_vec3_t ldir = {{ 1.f, 1.f, 1.f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        float now = ng_time_s();
        float dt = now - last;
        last = now;
        frame = (frame + 1) % FB_COUNT;

        /* --- Movement (same idea as L26) --- */
        float sx = (float)ng_dz(in.stick_x) / 80.f;
        float sy = (float)ng_dz(in.stick_y) / 80.f;
        float speed = sqrtf(sx * sx + sy * sy);
        if (speed > 1.f) {
            sx /= speed;
            sy /= speed;
            speed = 1.f;
        }

        float c = fm_cosf(camYaw), s = fm_sinf(camYaw);
        float mx = sx * c + sy * s;
        float mz = -sx * s + sy * c;

        /*
         * blend is the animation mix factor:
         *   0.0 → fully idle
         *   1.0 → fully walk
         * We use stick magnitude so a light push is a soft blend.
         */
        float blend = 0.f;

        if (speed > 0.15f) {
            pos.v[0] += mx * 7.f * dt;
            pos.v[2] += mz * 7.f * dt;
            yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.25f);
            blend = ng_clamp(speed, 0.f, 1.f);
        }
        pos.v[0] = ng_clamp(pos.v[0], -5.5f, 5.5f);
        pos.v[2] = ng_clamp(pos.v[2], -5.5f, 5.5f);

        /* --- Animation update (the heart of this lesson) --- */

        /* Advance both clips in time. They write into their attached skeletons. */
        t3d_anim_update(&animIdle, dt);

        /* Optional: make walk cycle rate follow how hard you push. */
        t3d_anim_set_speed(&animWalk, 0.2f + blend);
        t3d_anim_update(&animWalk, dt);

        /*
         * Blend poses, then rebuild bone matrices.
         * Forgetting skeleton_update is a classic "T-pose forever" bug.
         */
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        /* --- Camera + matrices + draw --- */
        fm_vec3_t eye = {{
            pos.v[0] + fm_sinf(camYaw) * 14.f,
            pos.v[1] + 8.f,
            pos.v[2] + fm_cosf(camYaw) * 14.f,
        }};
        fm_vec3_t target = {{ pos.v[0], pos.v[1] + 1.2f, pos.v[2] }};
        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.f), 1.f, 90.f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Character mesh is authored large — scale down for this island. */
        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.35f, 0.35f, 0.35f },
            (float[3]){ 0, yaw, 0 },
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(35, 55, 90, 0));
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_set(islandMat, true);
        if (island) {
            t3d_model_draw(island);
        }

        t3d_matrix_set(&playerMat[frame], true);
        /* NOTE: draw_skinned, not draw — uses the skeleton pose. */
        t3d_model_draw_skinned(player, &skel);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L27 — Anim from speed");
        snprintf(line, sizeof(line), "blend=%.2f (0=idle 1=walk)", blend);
        rdpq_text_print(NULL, 1, 10, 28, line);
        rdpq_text_print(NULL, 1, 10, 44, "Stop = idle, push stick = walk blend");

        rdpq_detach_show();
    }
}
