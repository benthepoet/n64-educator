/**
 * L31 — Game state machine
 *
 * NOOB MAP
 * --------
 * TITLE → PLAY ⇄ PAUSE → WIN
 * Start toggles pause in play; collect 3 shards to win; Start on title begins.
 *
 * docs/guide/m4/l31-game-state.md
 */

/**
 * NOTE FOR NOOBS
 * --------------
 * This file is a teaching step toward the full game. For *line-by-line*
 * commentary of the complete systems (audio + HUD + 8 shards + ranks),
 * read:
 *   capstone/starshard-cove/src/main.c
 *
 * That capstone file is intentionally over-commented as the course map.
 *
 */


#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <stdio.h>
#include "ng_game.h"

#define FB_COUNT 3
#define SHARD_N 3

typedef enum {
    ST_TITLE = 0,
    ST_PLAY,
    ST_PAUSE,
    ST_WIN,
} GameState;

typedef struct {
    fm_vec3_t pos;
    bool alive;
    T3DMat4FP *mat;
} Shard;

static float dist_xz(fm_vec3_t a, fm_vec3_t b)
{
    float dx = a.v[0] - b.v[0], dz = a.v[2] - b.v[2];
    return sqrtf(dx * dx + dz * dz);
}

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
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 0.032f, 0.032f, 0.032f }, (float[3]){ 0, 0, 0 }, (float[3]){ 0, 0, 0 });

    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");
    T3DModel *shardM = t3d_model_load("rom:/starshard.t3dm");
    assertf(island && player && shardM,
            "L31: failed to load island/player/starshard from ROM (DFS)");

    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);
    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);
    t3d_anim_attach(&animWalk, &skelBlend);

    Shard shards[SHARD_N];
    fm_vec3_t shardPos[SHARD_N] = {
        {{ 3.f, 0.65f, 2.f }}, {{-2.5f, 0.65f, 2.8f }}, {{ 1.2f, 0.65f, -3.2f }},
    };
    for (int i = 0; i < SHARD_N; i++) {
        shards[i].pos = shardPos[i];
        shards[i].alive = true;
        shards[i].mat = malloc_uncached(sizeof(T3DMat4FP));
    }

    GameState state = ST_TITLE;
    fm_vec3_t pos = {{ 0, 0.15f, 0 }};
    float yaw = 0.f, camYaw = 0.f;
    fm_vec3_t eye = {{ 0, 8, 14 }}, look = {{ 0, 1, 0 }};
    float last = ng_time_s();
    int frame = 0, collected = 0;
    float t = 0.f;
    char line[72];

    uint8_t amb[4] = { 80, 90, 100, 255 };
    uint8_t dirC[4] = { 255, 230, 200, 255 };
    fm_vec3_t ldir = {{ 0.3f, 1.f, 0.25f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        float dt = ng_time_s() - last;
        last = ng_time_s();
        if (dt < 0.f) {
            dt = 0.f;
        }
        if (dt > 0.1f) {
            dt = 0.1f;
        }
        frame = (frame + 1) % FB_COUNT;
        t += dt;

        /* ----- State transitions ----- */
        if (state == ST_TITLE && pressed.start) {
            state = ST_PLAY;
            collected = 0;
            pos = (fm_vec3_t){{ 0, 0.15f, 0 }};
            yaw = camYaw = 0.f;
            eye = (fm_vec3_t){{ 0, 8, 14 }};
            look = (fm_vec3_t){{ 0, 1, 0 }};
            for (int i = 0; i < SHARD_N; i++) {
                shards[i].alive = true;
            }
        } else if (state == ST_PLAY && pressed.start) {
            state = ST_PAUSE;
        } else if (state == ST_PAUSE && pressed.start) {
            state = ST_PLAY;
        } else if (state == ST_WIN && pressed.start) {
            state = ST_TITLE;
        }

        float blend = 0.f;
        if (state == ST_PLAY) {
            if (in.btn.c_left) {
                camYaw -= 1.2f * dt;
            }
            if (in.btn.c_right) {
                camYaw += 1.2f * dt;
            }
            float sx = (float)ng_dz(in.stick_x) / 80.f;
            float sy = (float)ng_dz(in.stick_y) / 80.f;
            float speed = sqrtf(sx * sx + sy * sy);
            if (speed > 1.f) {
                sx /= speed;
                sy /= speed;
                speed = 1.f;
            }
            /* Move relative to lagged camera (eye→player), not player yaw. */
            float edx = eye.v[0] - pos.v[0];
            float edz = eye.v[2] - pos.v[2];
            float elen = sqrtf(edx * edx + edz * edz);
            float c, s;
            if (elen > 0.001f) {
                float backNow = atan2f(edx, edz);
                c = fm_cosf(backNow);
                s = fm_sinf(backNow);
            } else {
                c = fm_cosf(camYaw);
                s = fm_sinf(camYaw);
            }
            float mx = sx * c - sy * s;
            float mz = -sx * s - sy * c;
            if (speed > 0.15f) {
                pos.v[0] += mx * 7.5f * dt;
                pos.v[2] += mz * 7.5f * dt;
                yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.22f);
                blend = speed;
            }
            pos.v[0] = ng_clamp(pos.v[0], -5.5f, 5.5f);
            pos.v[2] = ng_clamp(pos.v[2], -5.5f, 5.5f);
            for (int i = 0; i < SHARD_N; i++) {
                if (shards[i].alive && dist_xz(pos, shards[i].pos) < 1.1f) {
                    shards[i].alive = false;
                    collected++;
                    if (collected >= SHARD_N) {
                        state = ST_WIN;
                    }
                }
            }
        }

        /* Anims still tick so idle breathes on title */
        float animDt = (state == ST_PAUSE) ? 0.f : dt;
        t3d_anim_update(&animIdle, animDt);
        t3d_anim_set_speed(&animWalk, 0.2f + blend);
        t3d_anim_update(&animWalk, animDt);
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        /* Camera: follow in play; slow orbit showcase on title */
        fm_vec3_t eyeWant;
        fm_vec3_t lookWant;
        if (state == ST_TITLE) {
            eyeWant = (fm_vec3_t){{
                fm_sinf(t * 0.35f) * 16.f,
                10.f,
                fm_cosf(t * 0.35f) * 16.f,
            }};
            lookWant = (fm_vec3_t){{ 0, 0.8f, 0 }};
        } else {
            eyeWant = (fm_vec3_t){{
                pos.v[0] + fm_sinf(camYaw) * 12.f,
                pos.v[1] + 6.5f,
                pos.v[2] + fm_cosf(camYaw) * 12.f,
            }};
            lookWant = (fm_vec3_t){{ pos.v[0], pos.v[1] + 1.3f, pos.v[2] }};
        }
        float lag = ng_clamp(10.f * dt, 0.08f, 0.4f);
        for (int k = 0; k < 3; k++) {
            eye.v[k] = ng_lerp(eye.v[k], eyeWant.v[k], lag);
            look.v[k] = ng_lerp(look.v[k], lookWant.v[k], lag);
        }

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(58.f), 1.f, 200.f);
        t3d_viewport_look_at(&viewport, &eye, &look, &(fm_vec3_t){{ 0, 1, 0 }});

        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.02f, 0.02f, 0.02f },
            (float[3]){ 0.f, -yaw, 0.f }, /* snake faces -Z */
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });
        for (int i = 0; i < SHARD_N; i++) {
            if (!shards[i].alive) {
                continue;
            }
            float bob = fm_sinf(t * 3.f + i) * 0.12f;
            float sc = 0.02f;
            t3d_mat4fp_from_srt_euler(shards[i].mat,
                (float[3]){ sc, sc, sc },
                (float[3]){ 0.2f, t + i, 0.1f },
                (float[3]){ shards[i].pos.v[0], shards[i].pos.v[1] + bob, shards[i].pos.v[2] });
        }

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(25, 55, 100, 0xFF));
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_push(islandMat);
        t3d_model_draw(island);
        t3d_matrix_pop(1);

        for (int i = 0; i < SHARD_N; i++) {
            if (!shards[i].alive) {
                continue;
            }
            t3d_matrix_push(shards[i].mat);
            t3d_model_draw(shardM);
            t3d_matrix_pop(1);
        }

        /* Always draw player (idle on title too) so the scene is never empty. */
        t3d_skeleton_use(&skel);
        t3d_matrix_push(&playerMat[frame]);
        t3d_model_draw_skinned(player, &skel);
        t3d_matrix_pop(1);

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L31 — Game state");
        if (state == ST_TITLE) {
            rdpq_text_print(NULL, 1, 80, 100, "TOY COVE DEMO");
            rdpq_text_print(NULL, 1, 70, 120, "Press START");
        } else if (state == ST_PAUSE) {
            rdpq_text_print(NULL, 1, 110, 100, "PAUSED");
            rdpq_text_print(NULL, 1, 60, 120, "START = resume");
        } else if (state == ST_WIN) {
            rdpq_text_print(NULL, 1, 100, 100, "YOU WIN!");
            rdpq_text_print(NULL, 1, 50, 120, "START = title");
        } else {
            snprintf(line, sizeof(line), "Shards %d/%d  START=pause", collected, SHARD_N);
            rdpq_text_print(NULL, 1, 10, 28, line);
        }

        rdpq_detach_show();
    }
}
