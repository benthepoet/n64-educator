/**
 * L34 — Juice & feedback
 *
 * Collect 3 — feel the juice on a small island. Third-person move, follow cam,
 * idle/walk blend, audio, HUD, juice, title/pause/win states.
 *
 * docs/guide/m5/starshard-cove.md  +  L32–L36
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
#define SHARD_N  3

#define CH_SFX   0
#define CH_UI    1
#define CH_BGM   2

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
    float pop; /* juice: 1→0 scale punch after spawn / for remaining */
} Shard;

static float dist_xz(fm_vec3_t a, fm_vec3_t b)
{
    float dx = a.v[0] - b.v[0], dz = a.v[2] - b.v[2];
    return sqrtf(dx * dx + dz * dz);
}

static const fm_vec3_t SHARD_SPAWNS[SHARD_N] = {
    {{ 3.2f, 0.7f, 2.0f }},
    {{ -2.8f, 0.7f, 2.6f }},
    {{ 1.0f, 0.7f, -3.4f }},
};

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

    /* Audio */
    audio_init(48000, 4);
    mixer_init(16);
    wav64_init_compression(3);
    wav64_t *sfx_collect = wav64_load("rom:/collect.wav64", NULL);
    wav64_t *sfx_ui = wav64_load("rom:/ui.wav64", NULL);
    wav64_t *sfx_win = wav64_load("rom:/win.wav64", NULL);
    xm64player_t music;
    bool music_ok = true;
    xm64player_open(&music, "rom:/music.xm64");
    xm64player_set_loop(&music, true);
    xm64player_set_vol(&music, 0.55f);

    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);
    T3DMat4FP *playerMat = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 0.032f, 0.032f, 0.032f }, (float[3]){ 0, 0, 0 }, (float[3]){ 0, 0, 0 });

    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");
    T3DModel *shardM = t3d_model_load("rom:/starshard.t3dm");

    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);
    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);
    t3d_anim_attach(&animWalk, &skelBlend);

    Shard shards[SHARD_N];
    for (int i = 0; i < SHARD_N; i++) {
        shards[i].pos = SHARD_SPAWNS[i];
        shards[i].alive = true;
        shards[i].pop = 0.f;
        shards[i].mat = malloc_uncached(sizeof(T3DMat4FP));
    }

    GameState state = ST_TITLE;
    fm_vec3_t pos = {{ 0, 0.15f, 0 }};
    float yaw = 0.f, orbit = 0.f;
    fm_vec3_t eye = {{ 0, 8, 14 }}, look = {{ 0, 1, 0 }};
    float last = ng_time_s();
    int frame = 0, collected = 0;
    float t = 0.f, playTime = 0.f;
    float flash = 0.f;
    float winBanner = 0.f;
    char line[80];

    uint8_t amb[4] = { 80, 95, 110, 255 };
    uint8_t dirC[4] = { 255, 235, 210, 255 };
    fm_vec3_t ldir = {{ 0.35f, 1.f, 0.3f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        float dt = ng_time_s() - last;
        last = ng_time_s();
        if (dt > 0.1f) {
            dt = 0.1f;
        }
        frame = (frame + 1) % FB_COUNT;
        t += dt;
        if (flash > 0.f) {
            flash -= dt * 3.f;
        }
        if (winBanner > 0.f) {
            winBanner -= dt * 0.5f;
        }

        /* State transitions */
        if (state == ST_TITLE && pressed.start) {
            if (sfx_ui) {
                wav64_play(sfx_ui, CH_UI);
            }
            state = ST_PLAY;
            collected = 0;
            playTime = 0.f;
            pos = (fm_vec3_t){{ 0, 0.15f, 0 }};
            yaw = orbit = 0.f;
            for (int i = 0; i < SHARD_N; i++) {
                shards[i].alive = true;
                shards[i].pop = 0.f;
            }
            if (music_ok) {
                xm64player_play(&music, CH_BGM);
            }
        } else if (state == ST_PLAY && pressed.start) {
            if (sfx_ui) {
                wav64_play(sfx_ui, CH_UI);
            }
            state = ST_PAUSE;
            if (music_ok) {
                xm64player_stop(&music);
            }
        } else if (state == ST_PAUSE && pressed.start) {
            if (sfx_ui) {
                wav64_play(sfx_ui, CH_UI);
            }
            state = ST_PLAY;
            if (music_ok) {
                xm64player_play(&music, CH_BGM);
            }
        } else if (state == ST_WIN && pressed.start) {
            if (sfx_ui) {
                wav64_play(sfx_ui, CH_UI);
            }
            state = ST_TITLE;
            if (music_ok) {
                xm64player_stop(&music);
            }
        }

        float blend = 0.f;
        if (state == ST_PLAY) {
            playTime += dt;
            if (in.btn.c_left) {
                orbit -= 1.3f * dt;
            }
            if (in.btn.c_right) {
                orbit += 1.3f * dt;
            }

            float sx = (float)ng_dz(in.stick_x) / 80.f;
            float sy = (float)ng_dz(in.stick_y) / 80.f;
            float speed = sqrtf(sx * sx + sy * sy);
            if (speed > 1.f) {
                sx /= speed;
                sy /= speed;
                speed = 1.f;
            }
            float camF = yaw + orbit;
            float c = fm_cosf(camF), s = fm_sinf(camF);
            float mx = sx * c - sy * s;
            float mz = -sx * s - sy * c;
            if (speed > 0.15f) {
                pos.v[0] += mx * 7.8f * dt;
                pos.v[2] += mz * 7.8f * dt;
                yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.22f);
                blend = speed;
            }
            float rr = sqrtf(pos.v[0] * pos.v[0] + pos.v[2] * pos.v[2]);
            if (rr > 5.8f) {
                pos.v[0] *= 5.8f / rr;
                pos.v[2] *= 5.8f / rr;
            }

            for (int i = 0; i < SHARD_N; i++) {
                if (!shards[i].alive) {
                    continue;
                }
                if (dist_xz(pos, shards[i].pos) < 1.15f) {
                    shards[i].alive = false;
                    collected++;
                    flash = 1.f;
                    if (sfx_collect) {
                        wav64_play(sfx_collect, CH_SFX);
                    }
                    if (collected >= SHARD_N) {
                        state = ST_WIN;
                        winBanner = 1.f;
                        if (sfx_win) {
                            wav64_play(sfx_win, CH_SFX);
                        }
                        if (music_ok) {
                            xm64player_stop(&music);
                        }
                    }
                }
            }
        }

        float animDt = (state == ST_PAUSE) ? 0.f : dt;
        t3d_anim_update(&animIdle, animDt);
        t3d_anim_set_speed(&animWalk, 0.2f + blend);
        t3d_anim_update(&animWalk, animDt);
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        float back = yaw + orbit;
        fm_vec3_t eyeWant = {{
            pos.v[0] + fm_sinf(back) * 12.f, pos.v[1] + 6.5f,
            pos.v[2] + fm_cosf(back) * 12.f,
        }};
        fm_vec3_t lookWant = {{ pos.v[0], pos.v[1] + 1.35f, pos.v[2] }};
        if (state == ST_TITLE) {
            eyeWant = (fm_vec3_t){{ fm_sinf(t * 0.35f) * 15.f, 10.f, fm_cosf(t * 0.35f) * 15.f }};
            lookWant = (fm_vec3_t){{ 0, 0.4f, 0 }};
        }
        float lag = ng_clamp(10.f * dt, 0.08f, 0.4f);
        for (int k = 0; k < 3; k++) {
            eye.v[k] = ng_lerp(eye.v[k], eyeWant.v[k], lag);
            look.v[k] = ng_lerp(look.v[k], lookWant.v[k], lag);
        }

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(58.f), 1.f, 200.f);
        t3d_viewport_look_at(&viewport, &eye, &look, &(fm_vec3_t){{ 0, 1, 0 }});

        /*
         * Scales (course t3dm integer units → ~12u island):
         *   island    ~±384 → 0.032
         *   snake     ~height 80 → 0.02 (~1.6u tall)
         *   starshard ~height 100 → ~0.02 (~2u crystal)
         */
        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.02f, 0.02f, 0.02f },
            (float[3]){ 0.f, -yaw, 0.f },
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        for (int i = 0; i < SHARD_N; i++) {
            if (!shards[i].alive) {
                continue;
            }
            float bob = fm_sinf(t * 3.2f + i * 0.7f) * 0.14f;
            float spin = t * 1.5f + i;
            float sc = 0.02f * (1.0f + 0.08f * fm_sinf(t * 5.f + i));
            t3d_mat4fp_from_srt_euler(shards[i].mat,
                (float[3]){ sc, sc, sc },
                (float[3]){ 0.2f, spin, 0.1f },
                (float[3]){ shards[i].pos.v[0], shards[i].pos.v[1] + bob, shards[i].pos.v[2] });
        }

        /* ----- Draw 3D ----- */
        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);

        color_t clear = RGBA32(25, 55, 100, 0);
        if (flash > 0.f) {
            uint8_t add = (uint8_t)(flash * 80.f);
            clear = RGBA32(25 + add, 55 + add / 2, 100, 0);
        }
        t3d_screen_clear_color(clear);
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_push(islandMat);
        if (island) {
            t3d_model_draw(island);
        }
        t3d_matrix_pop(1);

        for (int i = 0; i < SHARD_N; i++) {
            if (!shards[i].alive) {
                continue;
            }
            t3d_matrix_push(shards[i].mat);
            if (shardM) {
                t3d_model_draw(shardM);
            }
            t3d_matrix_pop(1);
        }

        /* Always draw player (title shows idle on the island too). */
        t3d_skeleton_use(&skel);
        t3d_matrix_push(&playerMat[frame]);
        t3d_model_draw_skinned(player, &skel);
        t3d_matrix_pop(1);

        /* ----- HUD (2D over 3D) ----- */
        rdpq_set_mode_standard();
        if (state == ST_TITLE) {
            rdpq_text_print(NULL, 1, 70, 70, "L34 JUICE");
            rdpq_text_print(NULL, 1, 55, 100, "Collect 3 — feel the juice");
            rdpq_text_print(NULL, 1, 75, 130, "Press START");
            rdpq_text_print(NULL, 1, 40, 200, "Stick move  C-L/R cam  START pause");
        } else if (state == ST_PAUSE) {
            rdpq_text_print(NULL, 1, 120, 90, "PAUSED");
            rdpq_text_print(NULL, 1, 55, 120, "START resume");
            snprintf(line, sizeof(line), "Shards %d/%d  time %.1fs", collected, SHARD_N, playTime);
            rdpq_text_print(NULL, 1, 40, 150, line);
        } else if (state == ST_WIN) {
            rdpq_text_print(NULL, 1, 100, 70, "YOU WIN!");
            snprintf(line, sizeof(line), "Time: %.1f seconds", playTime);
            rdpq_text_print(NULL, 1, 70, 100, line);
            const char *rank = "Bronze";
            if (playTime < 45.f) {
                rank = "Gold";
            } else if (playTime < 90.f) {
                rank = "Silver";
            }
            snprintf(line, sizeof(line), "Rank: %s", rank);
            rdpq_text_print(NULL, 1, 100, 120, line);
            rdpq_text_print(NULL, 1, 55, 160, "START — title");
            rdpq_text_print(NULL, 1, 30, 200, "N64 Educator capstone");
        } else {
            /* PLAY HUD */
            snprintf(line, sizeof(line), "Shards %d/%d", collected, SHARD_N);
            rdpq_text_print(NULL, 1, 10, 12, line);
            snprintf(line, sizeof(line), "Time %.1f", playTime);
            rdpq_text_print(NULL, 1, 200, 12, line);
            rdpq_text_print(NULL, 1, 10, 220, "START pause");
        }

        rdpq_detach_show();
        mixer_try_play();
    }
}
