/**
 * L28 — Third-person follow camera
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Camera sits *behind the player* (based on player yaw + optional orbit),
 * with lag so turns feel soft — not glued 1:1 to the character.
 *
 * RECIPE
 * ------
 *   desired_eye  = player - forward*dist + up*height
 *   desired_look = player + head_offset
 *   eye  = lerp(eye, desired_eye, lag)
 *   look = lerp(look, desired_look, lag)
 *   t3d_viewport_look_at(eye, look, up)
 *
 * Move is still camera-relative (yaw + orbit), like a real 3rd-person game.
 *
 * CONTROLS: Stick move, C-left/right orbit
 * DOCS: docs/guide/m4/l28-follow-cam.md
 * Also see: capstone/starshard-cove/src/main.c (CAMERA section)
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
        (float[3]){ 0.032f, 0.032f, 0.032f }, (float[3]){ 0, 0, 0 }, (float[3]){ 0, 0, 0 });

    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");
    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);
    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);
    t3d_anim_attach(&animWalk, &skelBlend);

    fm_vec3_t pos = {{ 0, 0.15f, 0 }};
    float yaw = 0.f;
    float orbit = 0.f; /* extra camera orbit around player */
    fm_vec3_t eye = {{ 0, 8, 14 }};
    fm_vec3_t look = {{ 0, 1, 0 }};
    float last = ng_time_s();
    int frame = 0;

    uint8_t amb[4] = { 85, 90, 100, 255 };
    uint8_t dirC[4] = { 255, 220, 200, 255 };
    fm_vec3_t ldir = {{ 0.4f, 1.f, 0.3f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        float dt = ng_time_s() - last;
        last = ng_time_s();
        frame = (frame + 1) % FB_COUNT;

        if (in.btn.c_left) {
            orbit -= 1.2f * dt;
        }
        if (in.btn.c_right) {
            orbit += 1.2f * dt;
        }

        float sx = (float)ng_dz(in.stick_x) / 80.f;
        float sy = (float)ng_dz(in.stick_y) / 80.f;
        float speed = sqrtf(sx * sx + sy * sy);
        if (speed > 1.f) {
            sx /= speed;
            sy /= speed;
            speed = 1.f;
        }

        /* Move relative to camera facing (player yaw + orbit). */
        float camFacing = yaw + orbit;
        float c = fm_cosf(camFacing), s = fm_sinf(camFacing);
        float mx = sx * c - sy * s;
        float mz = -sx * s - sy * c;
        float blend = 0.f;
        if (speed > 0.15f) {
            pos.v[0] += mx * 7.5f * dt;
            pos.v[2] += mz * 7.5f * dt;
            yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.22f);
            blend = speed;
        }
        pos.v[0] = ng_clamp(pos.v[0], -5.5f, 5.5f);
        pos.v[2] = ng_clamp(pos.v[2], -5.5f, 5.5f);

        t3d_anim_update(&animIdle, dt);
        t3d_anim_set_speed(&animWalk, 0.2f + blend);
        t3d_anim_update(&animWalk, dt);
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        /* Desired camera behind player */
        float back = yaw + orbit;
        fm_vec3_t eyeWant = {{
            pos.v[0] + fm_sinf(back) * 12.f,
            pos.v[1] + 6.5f,
            pos.v[2] + fm_cosf(back) * 12.f,
        }};
        fm_vec3_t lookWant = {{ pos.v[0], pos.v[1] + 1.4f, pos.v[2] }};
        float lag = ng_clamp(10.f * dt, 0.08f, 0.4f);
        eye.v[0] = ng_lerp(eye.v[0], eyeWant.v[0], lag);
        eye.v[1] = ng_lerp(eye.v[1], eyeWant.v[1], lag);
        eye.v[2] = ng_lerp(eye.v[2], eyeWant.v[2], lag);
        look.v[0] = ng_lerp(look.v[0], lookWant.v[0], lag);
        look.v[1] = ng_lerp(look.v[1], lookWant.v[1], lag);
        look.v[2] = ng_lerp(look.v[2], lookWant.v[2], lag);

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(58.f), 1.f, 200.f);
        t3d_viewport_look_at(&viewport, &eye, &look, &(fm_vec3_t){{ 0, 1, 0 }});

        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.02f, 0.02f, 0.02f },
            (float[3]){ 0, yaw, 0 },
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(30, 60, 100, 0));
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_push(islandMat);
        if (island) {
            t3d_model_draw(island);
        }
        t3d_matrix_pop(1);
        t3d_skeleton_use(&skel);
            t3d_matrix_push(&playerMat[frame]);
            t3d_model_draw_skinned(player, &skel);
            t3d_matrix_pop(1);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L28 — Follow camera");
        rdpq_text_print(NULL, 1, 10, 28, "Stick move  C-left/right orbit lag cam");

        rdpq_detach_show();
    }
}
