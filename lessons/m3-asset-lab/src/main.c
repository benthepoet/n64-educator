/**
 * Module 3 — Asset lab
 * ============================================================================
 * Gallery for Blender-exported (or course) props.
 * D-pad L/R cycles models; stick orbits; A toggles idle/walk on player_anim.
 * Use this to validate glb → t3dm exports before gameplay systems.
 * Fully commented game wiring lives in capstone/starshard-cove/src/main.c
 * DOCS: docs/guide/m3/checkpoint.md
 */


#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <stdio.h>

#define FB_COUNT 3
#define DEADZONE 8

typedef enum {
    PROP_STARSHARD = 0,
    PROP_PLATFORM,
    PROP_ISLAND,
    PROP_PLAYER_STATIC,
    PROP_PLAYER_ANIM,
    PROP_COUNT
} PropId;

static const char *PROP_NAMES[PROP_COUNT] = {
    "starshard",
    "platform",
    "island",
    "player_static",
    "player_anim (idle/walk)",
};

static int dz(int v)
{
    return (v > -DEADZONE && v < DEADZONE) ? 0 : v;
}

static float get_time_s(void)
{
    return (float)((double)get_ticks_us() / 1000000.0);
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
    T3DMat4FP *modelMatFP = malloc_uncached(sizeof(T3DMat4FP) * FB_COUNT);

    T3DModel *models[PROP_COUNT] = {
        t3d_model_load("rom:/starshard.t3dm"),
        t3d_model_load("rom:/platform.t3dm"),
        t3d_model_load("rom:/island.t3dm"),
        t3d_model_load("rom:/player_static.t3dm"),
        t3d_model_load("rom:/player_anim.t3dm"),
    };

    /* Animated reference (snake) — optional if load failed */
    T3DSkeleton skel = { 0 };
    T3DSkeleton skelBlend = { 0 };
    T3DAnim animIdle = { 0 };
    T3DAnim animWalk = { 0 };
    bool hasAnim = false;
    if (models[PROP_PLAYER_ANIM]) {
        skel = t3d_skeleton_create_buffered(models[PROP_PLAYER_ANIM], FB_COUNT);
        skelBlend = t3d_skeleton_clone(&skel, false);
        animIdle = t3d_anim_create(models[PROP_PLAYER_ANIM], "Snake_Idle");
        animWalk = t3d_anim_create(models[PROP_PLAYER_ANIM], "Snake_Walk");
        t3d_anim_attach(&animIdle, &skel);
        t3d_anim_attach(&animWalk, &skelBlend);
        hasAnim = true;
    }

    PropId prop = PROP_STARSHARD;
    float yaw = 0.8f, pitch = 0.45f, dist = 35.f;
    bool walkMode = false;
    int frameIdx = 0;
    float lastTime = get_time_s();
    char line[80];

    uint8_t ambient[4] = { 90, 90, 110, 0xFF };
    uint8_t dirCol[4] = { 0xFF, 0xDD, 0xBB, 0xFF };
    fm_vec3_t lightDir = {{ 0.4f, 1.0f, 0.3f }};
    fm_vec3_norm(&lightDir, &lightDir);

    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        joypad_inputs_t in = joypad_get_inputs(JOYPAD_PORT_1);

        if (pressed.d_right) {
            prop = (PropId)((prop + 1) % PROP_COUNT);
        }
        if (pressed.d_left) {
            prop = (PropId)((prop + PROP_COUNT - 1) % PROP_COUNT);
        }
        if (pressed.a && hasAnim) {
            walkMode = !walkMode;
        }

        yaw += (float)dz(in.stick_x) * 0.0015f;
        pitch += (float)dz(in.stick_y) * 0.0012f;
        if (pitch < 0.1f) {
            pitch = 0.1f;
        }
        if (pitch > 1.4f) {
            pitch = 1.4f;
        }
        if (in.btn.c_up) {
            dist -= 0.4f;
        }
        if (in.btn.c_down) {
            dist += 0.4f;
        }
        if (dist < 8.f) {
            dist = 8.f;
        }
        if (dist > 100.f) {
            dist = 100.f;
        }

        float now = get_time_s();
        float dt = now - lastTime;
        lastTime = now;
        frameIdx = (frameIdx + 1) % FB_COUNT;

        if (hasAnim && prop == PROP_PLAYER_ANIM) {
            t3d_anim_update(&animIdle, dt);
            t3d_anim_update(&animWalk, dt);
            float blend = walkMode ? 1.0f : 0.0f;
            t3d_skeleton_blend(&skel, &skel, &skelBlend, blend);
            t3d_skeleton_update(&skel);
        }

        fm_vec3_t target = {{ 0, 0.5f, 0 }};
        fm_vec3_t eye = {{
            dist * fm_cosf(pitch) * fm_sinf(yaw),
            dist * fm_sinf(pitch) + 2.f,
            dist * fm_cosf(pitch) * fm_cosf(yaw),
        }};
        t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(60.0f), 2.0f, 200.0f);
        t3d_viewport_look_at(&viewport, &eye, &target, &(fm_vec3_t){{ 0, 1, 0 }});

        float scale = 1.0f;
        if (prop == PROP_ISLAND) {
            scale = 0.8f;
        } else if (prop == PROP_PLAYER_ANIM) {
            scale = 0.35f;
        } else if (prop == PROP_STARSHARD) {
            scale = 1.5f;
        }

        t3d_mat4fp_from_srt_euler(&modelMatFP[frameIdx],
            (float[3]){ scale, scale, scale },
            (float[3]){ 0, 0, 0 },
            (float[3]){ 0, 0, 0 });

        rdpq_attach(display_get(), display_get_zbuf());
        t3d_frame_start();
        t3d_viewport_attach(&viewport);
        t3d_screen_clear_color(RGBA32(35, 45, 70, 0));
        t3d_screen_clear_depth();

        t3d_light_set_ambient(ambient);
        t3d_light_set_directional(0, dirCol, &lightDir);
        t3d_light_set_count(1);

        t3d_matrix_set(&modelMatFP[frameIdx], true);
        if (models[prop]) {
            if (prop == PROP_PLAYER_ANIM && hasAnim) {
                t3d_skeleton_use(&skel);
                t3d_model_draw_skinned(models[prop], &skel);
            } else {
                t3d_model_draw(models[prop]);
            }
        }

        rdpq_set_mode_standard();
        rdpq_text_print(NULL, 1, 10, 12, "M3 Asset Lab");
        snprintf(line, sizeof(line), "Prop: %s", PROP_NAMES[prop]);
        rdpq_text_print(NULL, 1, 10, 28, line);
        rdpq_text_print(NULL, 1, 10, 44, "D-pad L/R change  Stick orbit  C dolly");
        if (prop == PROP_PLAYER_ANIM) {
            rdpq_text_print(NULL, 1, 10, 60,
                            walkMode ? "A: WALK blend" : "A: IDLE blend");
        }

        rdpq_detach_show();
    }
}
