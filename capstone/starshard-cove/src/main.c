/**
 * ============================================================================
 * Starshard Cove — full course capstone
 * ============================================================================
 *
 * WHAT THIS FILE IS
 * -----------------
 * One complete (small) 3D N64 game. If you can follow this file top-to-bottom,
 * you have seen every major idea from Modules 0–5 wired together.
 *
 * PLAYER FANTASY
 * --------------
 * Walk around a tiny island, collect 8 glowing starshards, try for a good time.
 *
 * HOW TO READ THIS FILE (noobs)
 * -----------------------------
 * 1. Skim the #defines and typedefs — names only.
 * 2. Jump to main() and read the big section comments in order:
 *      INIT → LOAD ASSETS → GAME LOOP → UPDATE → DRAW 3D → HUD → AUDIO MIX
 * 3. When something looks magical (skeleton, matrix, mixer), search this file
 *    for the matching "WHY" comment block above it.
 *
 * CONTROLS
 * --------
 *   START     Title → Play, Play ⇄ Pause, Win → Title
 *   Stick     Move (camera-relative)
 *   C-Left/Right   Orbit camera around the player
 *
 * BUILD
 * -----
 *   source scripts/env.sh && make -C capstone/starshard-cove
 *
 * Docs: docs/guide/m5/l35-capstone.md
 */

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <stdio.h>
#include "ng_game.h" /* deadzone, time, lerp helpers from Module 4 */

/* -------------------------------------------------------------------------- */
/*  Tunables — change these while learning; recompile and feel the difference */
/* -------------------------------------------------------------------------- */

/** How many color+depth framebuffers. 3 lets the CPU work a bit ahead of the RDP. */
#define FB_COUNT 3

/** How many starshards to collect for a win. */
#define SHARD_N  8

/**
 * Mixer channel numbers.
 * The mixer can play several sounds at once; each needs a channel index.
 * Music (XM) may use several channels starting at CH_BGM — leave room after it.
 */
#define CH_SFX   0  /* one-shot collect / win */
#define CH_UI    1  /* menu blips */
#define CH_BGM   2  /* XM music starts here */

/** Walk speed in world units per second (island is roughly radius ~6). */
#define MOVE_SPEED 7.8f

/** Distance (in XZ plane) at which a shard is collected. */
#define PICKUP_RADIUS 1.15f

/* -------------------------------------------------------------------------- */
/*  Game state machine                                                        */
/* -------------------------------------------------------------------------- */

/**
 * The game is always in exactly one of these modes.
 * Only ST_PLAY runs movement and pickups. That prevents “paused but still moving”
 * bugs that confuse beginners forever.
 *
 *   TITLE --START--> PLAY <--> PAUSE
 *                      |
 *                   8 shards
 *                      v
 *                     WIN --START--> TITLE
 */
typedef enum {
    ST_TITLE = 0,
    ST_PLAY,
    ST_PAUSE,
    ST_WIN,
} GameState;

/* -------------------------------------------------------------------------- */
/*  Entities                                                                  */
/* -------------------------------------------------------------------------- */

/**
 * One collectible starshard in the world.
 *
 *  pos    — world position (y is height above the island surface-ish)
 *  alive  — false after the player picks it up (we stop drawing it)
 *  mat    — fixed-point model matrix the RSP reads (must be uncached memory)
 *  pop    — reserved for juice (scale punch); currently unused but kept for demos
 */
typedef struct {
    fm_vec3_t pos;
    bool alive;
    T3DMat4FP *mat;
    float pop;
} Shard;

/**
 * Horizontal distance between two points (ignore height).
 * Good enough for pickups on a mostly flat island.
 */
static float dist_xz(fm_vec3_t a, fm_vec3_t b)
{
    float dx = a.v[0] - b.v[0];
    float dz = a.v[2] - b.v[2];
    return sqrtf(dx * dx + dz * dz);
}

/**
 * Spawn table — where each of the 8 shards starts.
 * In a bigger pipeline this might come from Blender empties (Module 3/4 idea).
 * Coordinates are in the same units as the island mesh.
 */
static const fm_vec3_t SHARD_SPAWNS[SHARD_N] = {
    {{ 3.2f, 0.7f, 2.0f }},
    {{ -2.8f, 0.7f, 2.6f }},
    {{ 1.0f, 0.7f, -3.4f }},
    {{ -3.5f, 0.7f, -1.5f }},
    {{ 4.0f, 0.7f, -0.5f }},
    {{ -1.2f, 0.7f, 4.0f }},
    {{ 2.0f, 0.7f, 4.2f }},
    {{ 0.0f, 0.7f, -4.5f }},
};

/* ========================================================================== */
int main(void)
/* ========================================================================== */
{
    /* ---------------------------------------------------------------------- */
    /* INIT — libdragon subsystems (Module 0 patterns)                        */
    /* ---------------------------------------------------------------------- */

    /* Optional: print debugf() to emulator / USB logger when available. */
    debug_init_isviewer();
    debug_init_usblog();

    /*
     * asset_init_compression: some ROM files (models, wav64) may be compressed
     * by mkasset/audioconv. This registers decompressors. Level 2 is common.
     */
    asset_init_compression(2);

    /*
     * dfs_init: mount the in-ROM filesystem so fopen("rom:/…") / loaders work.
     * Without this, every rom:/ path fails.
     */
    dfs_init(DFS_DEFAULT_LOCATION);

    /*
     * display_init: resolution, color depth, number of framebuffers.
     * FILTERS_RESAMPLE: bilinear VI scale without AA (AA can leave a 1px edge artifact).
     */
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, FB_COUNT, GAMMA_NONE,
                 FILTERS_RESAMPLE);

    rdpq_init();   /* RDP command helpers (clear, text, combiners, …) */
    joypad_init(); /* controllers (Module 0 L03) */

    /* Built-in debug font as font id 1 for HUD strings. */
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    /*
     * t3d_init: start Tiny3D (RSP microcode + helpers). Empty params = defaults.
     * Must happen before any t3d_* drawing.
     */
    t3d_init((T3DInitParams){});

    /* ---------------------------------------------------------------------- */
    /* AUDIO — mixer + SFX + music (Module 5 L32)                             */
    /* ---------------------------------------------------------------------- */

    /*
     * audio_init(freq, num_buffers): set up the audio subsystem.
     * mixer_init(channels): how many simultaneous voices.
     * wav64_init_compression(3): Opus — match Makefile --wav-compress 3.
     * Use 48000: Opus wav64 samples are 48 kHz (not 44100).
     */
    audio_init(48000, 4);
    mixer_init(16);
    wav64_init_compression(3);

    /* Load one-shot sound effects from the ROM filesystem. */
    wav64_t *sfx_collect = wav64_load("rom:/collect.wav64", NULL);
    wav64_t *sfx_ui = wav64_load("rom:/ui.wav64", NULL);
    wav64_t *sfx_win = wav64_load("rom:/win.wav64", NULL);

    /*
     * XM module music — tracker format, very N64-friendly size.
     * xm64player_* wraps the mixer for multi-channel music.
     */
    xm64player_t music;
    bool music_ok = true;
    xm64player_open(&music, "rom:/music.xm64");
    xm64player_set_loop(&music, true);
    xm64player_set_vol(&music, 0.55f); /* keep music under SFX */

    /* ---------------------------------------------------------------------- */
    /* 3D SCENE OBJECTS                                                       */
    /* ---------------------------------------------------------------------- */

    /*
     * Viewport holds projection (lens: FOV, near, far) and view (camera).
     * Buffered = one set of matrices per framebuffer (avoids RSP/CPU races).
     */
    T3DViewport viewport = t3d_viewport_create_buffered(FB_COUNT);

    /*
     * Model matrices for the RSP must live in *uncached* RAM so DMA sees
     * what we wrote. Using normal malloc can cause “matrix didn’t update” bugs.
     */
    T3DMat4FP *playerMat = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);
    T3DMat4FP *islandMat = malloc_uncached(sizeof(T3DMat4FP));

    /* Island never moves — build its matrix once (scale 1, no rotation, origin). */
    t3d_mat4fp_from_srt_euler(islandMat,
        (float[3]){ 0.032f, 0.032f, 0.032f }, /* scale XYZ — island t3dm ~±384 */
        (float[3]){ 0, 0, 0 },   /* rotation XYZ (radians) */
        (float[3]){ 0, 0, 0 });  /* translation XYZ */

    /* Load meshes converted at build time (Module 2 L15 / Module 3). */
    T3DModel *island = t3d_model_load("rom:/island.t3dm");
    T3DModel *player = t3d_model_load("rom:/player_anim.t3dm");
    T3DModel *shardM = t3d_model_load("rom:/starshard.t3dm");

    /*
     * Skeleton + animations (Module 3 L25, Module 4 L27)
     * -----------------------------------------------
     * skel       — the pose we draw
     * skelBlend  — a second pose we use only for blending walk into idle
     * animIdle / animWalk — runtime players for named clips inside the model
     *
     * Clip names come from the glTF export (here: Snake_Idle / Snake_Walk).
     */
    T3DSkeleton skel = t3d_skeleton_create_buffered(player, FB_COUNT);
    T3DSkeleton skelBlend = t3d_skeleton_clone(&skel, false);
    T3DAnim animIdle = t3d_anim_create(player, "Snake_Idle");
    T3DAnim animWalk = t3d_anim_create(player, "Snake_Walk");
    t3d_anim_attach(&animIdle, &skel);       /* idle writes into skel */
    t3d_anim_attach(&animWalk, &skelBlend);  /* walk writes into skelBlend */

    /* Create shard entities from the spawn table. */
    Shard shards[SHARD_N];
    for (int i = 0; i < SHARD_N; i++) {
        shards[i].pos = SHARD_SPAWNS[i];
        shards[i].alive = true;
        shards[i].pop = 0.f;
        shards[i].mat = malloc_uncached(sizeof(T3DMat4FP));
    }

    /* ---------------------------------------------------------------------- */
    /* Runtime variables that change every frame                              */
    /* ---------------------------------------------------------------------- */

    GameState state = ST_TITLE;

    fm_vec3_t pos = {{ 0, 0.15f, 0 }}; /* player feet position (y slightly above ground) */
    float yaw = 0.f;   /* which way the player faces (radians, around Y) */
    float camYaw = 0.f; /* camera yaw (C-orbit); independent of player facing */

    /* Smooth camera state (lerped toward targets each frame). */
    fm_vec3_t eye = {{ 0, 8, 14 }};
    fm_vec3_t look = {{ 0, 1, 0 }};

    float last = ng_time_s(); /* for delta time */
    int frame = 0;            /* cycles 0..FB_COUNT-1 for matrix buffering */
    int collected = 0;        /* shards grabbed this run */
    float t = 0.f;            /* free-running time for bobbing / title orbit */
    float playTime = 0.f;     /* only counts in ST_PLAY — used for ranks */
    float flash = 0.f;        /* 1→0 after collect; brightens clear color (juice) */
    float winBanner = 0.f;    /* reserved juice timer on win */
    char line[80];            /* snprintf buffer for HUD */

    /* Lighting: soft ambient fill + one directional “sun”. */
    uint8_t amb[4] = { 80, 95, 110, 255 };
    uint8_t dirC[4] = { 255, 235, 210, 255 };
    fm_vec3_t ldir = {{ 0.35f, 1.f, 0.3f }};
    fm_vec3_norm(&ldir, &ldir); /* lights want a unit-length direction */

    /* ====================================================================== */
    /*  MAIN LOOP — runs forever (N64 games almost never return from main)    */
    /* ====================================================================== */
    for (;;) {
        /* ------------------------------------------------------------------ */
        /* INPUT + TIMING                                                     */
        /* ------------------------------------------------------------------ */

        joypad_poll(); /* always once per frame before reading pads */
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        /*
         * Delta time (dt) = seconds since last frame.
         * Cap it so a hitch doesn’t teleport the player across the island.
         */
        float dt = ng_time_s() - last;
        last = ng_time_s();
        if (dt > 0.1f) {
            dt = 0.1f;
        }

        frame = (frame + 1) % FB_COUNT;
        t += dt;
        if (flash > 0.f) {
            flash -= dt * 3.f; /* fade juice flash */
        }
        if (winBanner > 0.f) {
            winBanner -= dt * 0.5f;
        }

        /* ------------------------------------------------------------------ */
        /* STATE MACHINE — menus and mode changes only                        */
        /* ------------------------------------------------------------------ */

        if (state == ST_TITLE && pressed.start) {
            /* Begin a new run */
            if (sfx_ui) {
                wav64_play(sfx_ui, CH_UI);
            }
            state = ST_PLAY;
            collected = 0;
            playTime = 0.f;
            pos = (fm_vec3_t){{ 0, 0.15f, 0 }};
            yaw = camYaw = 0.f;
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

        /* ------------------------------------------------------------------ */
        /* GAMEPLAY UPDATE — only while playing                               */
        /* ------------------------------------------------------------------ */

        float blend = 0.f; /* 0 = full idle, 1 = full walk (for skeleton blend) */

        if (state == ST_PLAY) {
            playTime += dt;

            /* Optional camera orbit (does not turn the player by itself). */
            if (in.btn.c_left) {
                camYaw -= 1.3f * dt;
            }
            if (in.btn.c_right) {
                camYaw += 1.3f * dt;
            }

            /*
             * Stick → camera-relative move (Module 4 L28+)
             * --------------------------------------------
             * Stick X/Y are roughly -80..80. Normalize to about -1..1.
             * Rotate by the *lagged* camera (eye→player on XZ) so stick-up
             * matches what is on screen. camYaw is only for C-orbit / fallback.
             * Never use player yaw as the move basis (strafe feedback-spins).
             */
            float sx = (float)ng_dz(in.stick_x) / 80.f;
            float sy = (float)ng_dz(in.stick_y) / 80.f;
            float speed = sqrtf(sx * sx + sy * sy);
            if (speed > 1.f) {
                sx /= speed;
                sy /= speed;
                speed = 1.f;
            }

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
                pos.v[0] += mx * MOVE_SPEED * dt;
                pos.v[2] += mz * MOVE_SPEED * dt;
                /* Face movement direction (model only — does not drive camYaw). */
                yaw = ng_lerp_angle(yaw, atan2f(mx, mz), 0.22f);

                blend = speed; /* drive walk animation amount */
            }

            /*
             * Soft wall: axis-aligned box, not a radial cylinder.
             * Radial clamp skates you around the rim and feels like camera spin.
             * Not real mesh collision — good enough for this mini-game.
             */
            pos.v[0] = ng_clamp(pos.v[0], -5.5f, 5.5f);
            pos.v[2] = ng_clamp(pos.v[2], -5.5f, 5.5f);

            /* Sphere-ish pickups in XZ (Module 4 L29). */
            for (int i = 0; i < SHARD_N; i++) {
                if (!shards[i].alive) {
                    continue;
                }
                if (dist_xz(pos, shards[i].pos) < PICKUP_RADIUS) {
                    shards[i].alive = false;
                    collected++;
                    flash = 1.f; /* juice: flash the screen */
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

        /* ------------------------------------------------------------------ */
        /* ANIMATION — advance clips and blend into the draw skeleton         */
        /* ------------------------------------------------------------------ */

        /* Freeze anims while paused so the character doesn’t moonwalk in place. */
        float animDt = (state == ST_PAUSE) ? 0.f : dt;

        t3d_anim_update(&animIdle, animDt);
        t3d_anim_set_speed(&animWalk, 0.2f + blend); /* walk a bit faster when moving */
        t3d_anim_update(&animWalk, animDt);

        /*
         * Blend: result = lerp(idlePose, walkPose, blend).
         * Then skeleton_update builds bone matrices for the skinned draw.
         */
        t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
        t3d_skeleton_update(&skel);

        /* ------------------------------------------------------------------ */
        /* CAMERA — third-person follow with lag (Module 4 L28)               */
        /* ------------------------------------------------------------------ */

        float back = camYaw; /* desired orbit sits on camYaw, not player yaw */
        fm_vec3_t eyeWant = {{
            pos.v[0] + fm_sinf(back) * 12.f, /* along camYaw (free orbit) */
            pos.v[1] + 6.5f,                 /* above */
            pos.v[2] + fm_cosf(back) * 12.f,
        }};
        fm_vec3_t lookWant = {{ pos.v[0], pos.v[1] + 1.35f, pos.v[2] }}; /* look at torso */

        if (state == ST_TITLE) {
            /* Pretty orbit around the island on the title screen. */
            eyeWant = (fm_vec3_t){{
                fm_sinf(t * 0.35f) * 15.f,
                10.f,
                fm_cosf(t * 0.35f) * 15.f,
            }};
            lookWant = (fm_vec3_t){{ 0, 0.4f, 0 }};
        }

        /* Smooth follow — higher lag factor = snappier camera. */
        float lag = ng_clamp(10.f * dt, 0.08f, 0.4f);
        for (int k = 0; k < 3; k++) {
            eye.v[k] = ng_lerp(eye.v[k], eyeWant.v[k], lag);
            look.v[k] = ng_lerp(look.v[k], lookWant.v[k], lag);
        }

        /*
         * Projection = lens (FOV 58°, near 1, far 200).
         * look_at builds the view matrix from eye, target, and world-up.
         */
        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(58.f), 1.f, 200.f);
        t3d_viewport_look_at(&viewport, &eye, &look, &(fm_vec3_t){{ 0, 1, 0 }});

        /* Player model matrix: small scale, face yaw, sit at pos. */
        t3d_mat4fp_from_srt_euler(&playerMat[frame],
            (float[3]){ 0.02f, 0.02f, 0.02f },
            (float[3]){ 0.f, -yaw, 0.f }, /* snake faces -Z */
            (float[3]){ pos.v[0], pos.v[1], pos.v[2] });

        /* Shard matrices: bob + spin for readability (Module 5 juice). */
        for (int i = 0; i < SHARD_N; i++) {
            if (!shards[i].alive) {
                continue;
            }
            float bob = fm_sinf(t * 3.2f + i * 0.7f) * 0.14f;
            float spin = t * 1.5f + i;
            float sc = 1.15f + 0.08f * fm_sinf(t * 5.f + i);
            t3d_mat4fp_from_srt_euler(shards[i].mat,
                (float[3]){ sc * 0.018f, sc * 0.018f, sc * 0.018f },
                (float[3]){ 0.2f, spin, 0.1f },
                (float[3]){ shards[i].pos.v[0], shards[i].pos.v[1] + bob, shards[i].pos.v[2] });
        }

        /* ------------------------------------------------------------------ */
        /* DRAW 3D                                                            */
        /* ------------------------------------------------------------------ */

        /*
         * Attach the next color framebuffer AND the depth (Z) buffer.
         * 3D needs depth so nearer triangles win (Module 2 L12).
         */
        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();           /* Tiny3D: once per frame before draws */
        t3d_viewport_attach(&viewport);

        /* Clear color — optionally flash white-ish after a pickup. */
        color_t clear = RGBA32(25, 55, 100, 0xFF);
        if (flash > 0.f) {
            uint8_t add = (uint8_t)(flash * 80.f);
            clear = RGBA32(25 + add, 55 + add / 2, 100, 0xFF);
        }
        t3d_screen_clear_color(clear);
        t3d_screen_clear_depth(); /* NEVER skip this or you get ghost geometry */

        t3d_light_set_ambient(amb);
        t3d_light_set_directional(0, dirC, &ldir);
        t3d_light_set_count(1);

        /* Ground first, then props, then player (order is less critical with Z). */
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

        /* Title screen: show empty island only (no player). */
        if (state != ST_TITLE) {
            /* PUSH so the model composes with the camera matrix on the stack
             * (matrix_set would overwrite it → blank mesh). */
            t3d_matrix_push(&playerMat[frame]);
            /* Skinned draw uses the skeleton pose we updated above. */
            t3d_skeleton_use(&skel);
            t3d_model_draw_skinned(player, &skel);
            t3d_matrix_pop(1);
        }

        /* ------------------------------------------------------------------ */
        /* HUD — 2D text AFTER 3D (Module 5 L33)                              */
        /* ------------------------------------------------------------------ */

        rdpq_set_mode_standard(); /* switch RDP out of pure 3D assumptions */

        if (state == ST_TITLE) {
            rdpq_text_print(NULL, 1, 70, 70, "STARSHARD COVE");
            rdpq_text_print(NULL, 1, 55, 100, "Collect 8 starshards");
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
            /* Soft ranks — not a hard fail, just bragging rights. */
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
            /* ST_PLAY — keep HUD minimal so the 3D scene stays readable. */
            snprintf(line, sizeof(line), "Shards %d/%d", collected, SHARD_N);
            rdpq_text_print(NULL, 1, 10, 12, line);
            snprintf(line, sizeof(line), "Time %.1f", playTime);
            rdpq_text_print(NULL, 1, 200, 12, line);
            rdpq_text_print(NULL, 1, 10, 220, "START pause");
        }

        /*
         * Present this framebuffer when ready (vsync-style wait is inside
         * display_get on the next iteration — Module 0 L02).
         */
        rdpq_detach_show();

        /*
         * Feed the audio mixer. Must run every frame or sound stutters/stops.
         * Safe to call even when nothing is playing.
         */
        mixer_try_play();
    }
}
