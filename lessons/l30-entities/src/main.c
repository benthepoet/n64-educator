/**
 * L30 — Entities & spawn table
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Stop scattering globals like "shard1_x", "shard2_x".
 * Instead:
 *   const Spawn SPAWNS[] = { {type, x,y,z}, ... };
 *   Entity ents[MAX]; filled from SPAWNS at boot
 *   each frame: update all ents, draw all alive ents
 *
 * Later you can fill SPAWNS from Blender empties or a JSON/CSV export.
 *
 * Entity types here: PLAYER, SHARD, PLATFORM
 * DOCS: docs/guide/m4/l30-entities.md
 */


#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <stdio.h>
#include "ng_game.h"

#define FB_COUNT 3
#define MAX_ENT 16

typedef enum {
    ENT_PLAYER = 0,
    ENT_SHARD,
    ENT_PLATFORM,
} EntType;

typedef struct {
    EntType type;
    fm_vec3_t pos;
    float yaw;
    bool alive;
    T3DMat4FP *mat; /* uncached, one slot (or buffered for player) */
} Entity;

typedef struct {
    EntType type;
    float x, y, z;
} Spawn;

static const Spawn SPAWNS[] = {
    { ENT_PLAYER, 0.f, 0.15f, 0.f },
    { ENT_SHARD, 3.2f, 0.7f, 1.5f },
    { ENT_SHARD, -3.f, 0.7f, 2.5f },
    { ENT_SHARD, 0.5f, 0.7f, -3.2f },
    { ENT_PLATFORM, 4.f, 0.3f, -2.f },
};
#define SPAWN_COUNT (sizeof(SPAWNS) / sizeof(SPAWNS[0]))

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
                 FILTERS_RESAMPLE_ANTIALIAS);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    t3d_init((T3DInitParams){});

    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 1, 1, 1 }, (float[3]){ 0, 0, 0 }, (float[3]){ 0, 0, 0 });

    T3DModel *mdlIsland = t3d_model_load("rom:/island.t3dm");
    T3DModel *mdlPlayer = t3d_model_load("rom:/player_anim.t3dm");
    T3DModel *mdlShard = t3d_model_load("rom:/starshard.t3dm");
    T3DModel *mdlPlat = t3d_model_load("rom:/platform.t3dm");

    T3DSkeleton skel = t3d_skeleton_create_buffered(mdlPlayer, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);
    T3DAnim animIdle = t3d_anim_create(mdlPlayer, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(mdlPlayer, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);
    t3d_anim_attach(&animWalk, &skelBlend);

    Entity ents[MAX_ENT];
    int entCount = 0;
    int playerIdx = 0;
    T3DMat4FP *playerMats = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);

    for (int i = 0; i < (int)SPAWN_COUNT; i++) {
        Entity *e = &ents[entCount++];
        e->type = SPAWNS[i].type;
        e->pos = (fm_vec3_t){{ SPAWNS[i].x, SPAWNS[i].y, SPAWNS[i].z }};
        e->yaw = 0.f;
        e->alive = true;
        if (e->type == ENT_PLAYER) {
            playerIdx = entCount - 1;
            e->mat = playerMats; /* special: buffered */
        } else {
            e->mat = malloc_uncached(sizeof(T3DMat4FP));
        }
    }

    float orbit = 0.f;
    fm_vec3_t eye = {{ 0, 8, 14 }};
    fm_vec3_t look = {{ 0, 1, 0 }};
    float last = ng_time_s();
    int frame = 0;
    int collected = 0;
    float t = 0.f;
    char line[80];

    uint8_t amb[4] = { 85, 90, 105, 255 };
    uint8_t dirC[4] = { 255, 225, 200, 255 };
    fm_vec3_t ldir = {{ 0.35f, 1.f, 0.2f }};
    fm_vec3_norm(&ldir, &ldir);

    for (;;) {
        joypad_poll();
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);
        float dt = ng_time_s() - last;
        last = ng_time_s();
        frame = (frame + 1) % FB_COUNT;
        t += dt;

        Entity *pl = &ents[playerIdx];
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
        float camF = pl->yaw + orbit;
        float c = fm_cosf(camF), s = fm_sinf(camF);
        float mx = sx * c + sy * s;
        float mz = -sx * s + sy * c;
        float blend = 0.f;
        if (speed > 0.15f) {
            pl->pos.v[0] += mx * 7.5f * dt;
            pl->pos.v[2] += mz * 7.5f * dt;
            pl->yaw = ng_lerp_angle(pl->yaw, atan2f(mx, mz), 0.22f);
            blend = speed;
        }
        float rr = sqrtf(pl->pos.v[0] * pl->pos.v[0] + pl->pos.v[2] * pl->pos.v[2]);
        if (rr > 5.8f) {
            pl->pos.v[0] *= 5.8f / rr;
            pl->pos.v[2] *= 5.8f / rr;
        }

        for (int i = 0; i < entCount; i++) {
            Entity *e = &ents[i];
            if (!e->alive || e->type != ENT_SHARD) {
                continue;
            }
            if (dist_xz(pl->pos, e->pos) < 1.1f) {
                e->alive = false;
                collected++;
            }
        }

        t3d_anim_update(&animIdle, dt);
        t3d_anim_set_speed(&animWalk, 0.2f + blend);
        t3d_anim_update(&animWalk, dt);
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        float back = pl->yaw + orbit;
        fm_vec3_t eyeWant = {{
            pl->pos.v[0] + fm_sinf(back) * 12.f, pl->pos.v[1] + 6.5f,
            pl->pos.v[2] + fm_cosf(back) * 12.f,
        }};
        fm_vec3_t lookWant = {{ pl->pos.v[0], pl->pos.v[1] + 1.3f, pl->pos.v[2] }};
        float lag = ng_clamp(10.f * dt, 0.08f, 0.4f);
        for (int k = 0; k < 3; k++) {
            eye.v[k] = ng_lerp(eye.v[k], eyeWant.v[k], lag);
            look.v[k] = ng_lerp(look.v[k], lookWant.v[k], lag);
        }

        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(58.f), 1.f, 90.f);
        t3d_viewport_look_at(&viewport, &eye, &look, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Build matrices from entities */
        for (int i = 0; i < entCount; i++) {
            Entity *e = &ents[i];
            if (!e->alive) {
                continue;
            }
            if (e->type == ENT_PLAYER) {
                t3d_mat4fp_from_srt_euler(&playerMats[frame],
                    (float[3]){ 0.35f, 0.35f, 0.35f },
                    (float[3]){ 0, e->yaw, 0 },
                    (float[3]){ e->pos.v[0], e->pos.v[1], e->pos.v[2] });
            } else if (e->type == ENT_SHARD) {
                float bob = fm_sinf(t * 3.f + i) * 0.12f;
                t3d_mat4fp_from_srt_euler(e->mat,
                    (float[3]){ 1.2f, 1.2f, 1.2f },
                    (float[3]){ 0, t + i, 0 },
                    (float[3]){ e->pos.v[0], e->pos.v[1] + bob, e->pos.v[2] });
            } else if (e->type == ENT_PLATFORM) {
                t3d_mat4fp_from_srt_euler(e->mat,
                    (float[3]){ 0.45f, 0.45f, 0.45f },
                    (float[3]){ 0, 0, 0 },
                    (float[3]){ e->pos.v[0], e->pos.v[1], e->pos.v[2] });
            }
        }

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(30, 55, 95, 0));
        t3d_screen_clear_depth();
        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        t3d_matrix_set(islandMat, true);
        if (mdlIsland) {
            t3d_model_draw(mdlIsland);
        }

        for (int i = 0; i < entCount; i++) {
            Entity *e = &ents[i];
            if (!e->alive) {
                continue;
            }
            if (e->type == ENT_PLAYER) {
                t3d_matrix_set(&playerMats[frame], true);
                t3d_model_draw_skinned(mdlPlayer, &skel);
            } else if (e->type == ENT_SHARD) {
                t3d_matrix_set(e->mat, true);
                if (mdlShard) {
                    t3d_model_draw(mdlShard);
                }
            } else if (e->type == ENT_PLATFORM) {
                t3d_matrix_set(e->mat, true);
                if (mdlPlat) {
                    t3d_model_draw(mdlPlat);
                }
            }
        }

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "L30 — Entity spawn table");
        snprintf(line, sizeof(line), "ents=%d  shards=%d  (from SPAWNS[])", entCount, collected);
        rdpq_text_print(NULL, 1, 10, 28, line);

        rdpq_detach_show();
    }
}
