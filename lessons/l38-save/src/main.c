/**
 * L38 — Save data & high scores (EEPROM / eepromfs)
 * ============================================================================
 *
 * LEARNING GOAL
 * -------------
 * Ship something that *remembers*:
 *   eepfs_init → verify signature (wipe if foreign) → read/write a small struct
 *
 * CONTROLS
 * --------
 *   A     start / finish a timed "run"
 *   START pause the run timer
 *   Z     wipe save (best time)
 *
 * Ares / flashcarts: Makefile sets N64_ROM_SAVETYPE=eeprom4k so the header
 * requests EEPROM. Still probe with eeprom_present() and handle NONE.
 *
 * BUILD: make -C lessons/l38-save
 * DOCS:  docs/guide/m5/l38-save.md
 */

#include <libdragon.h>
#include <stdio.h>
#include <string.h>

#define SAVE_PATH "best.dat"
#define MAGIC     0x4E473138u /* 'NG18' */

typedef struct {
    uint32_t magic;
    float best_time; /* seconds; large means "no best yet" */
    uint32_t runs;
} SaveBlob;

static const eepfs_entry_t FS_FILES[] = {
    { .path = SAVE_PATH, .size = sizeof(SaveBlob), .checksum = true, .backup = true },
};

static SaveBlob g_save = { .magic = MAGIC, .best_time = 1e9f, .runs = 0 };
static bool g_fs_ok = false;

static void save_defaults(void)
{
    g_save.magic = MAGIC;
    g_save.best_time = 1e9f;
    g_save.runs = 0;
}

static void save_load(void)
{
    if (!g_fs_ok) {
        return;
    }
    SaveBlob tmp;
    memset(&tmp, 0, sizeof(tmp));
    int r = eepfs_read(SAVE_PATH, &tmp, sizeof(tmp));
    if (r == EEPFS_ESUCCESS && tmp.magic == MAGIC) {
        g_save = tmp;
        debugf("L38: loaded best=%.2f runs=%lu\n",
               (double)g_save.best_time, (unsigned long)g_save.runs);
    } else {
        debugf("L38: no valid save (r=%d) — using defaults\n", r);
        save_defaults();
    }
}

static void save_store(void)
{
    if (!g_fs_ok) {
        return;
    }
    g_save.magic = MAGIC;
    int r = eepfs_write(SAVE_PATH, &g_save, sizeof(g_save));
    debugf("L38: write r=%d best=%.2f runs=%lu\n",
           r, (double)g_save.best_time, (unsigned long)g_save.runs);
    /* Wait for background EEPROM flush so power-off is safer. */
    while (eeprom_is_busy()) {
        /* spin */
    }
}

static void save_wipe(void)
{
    if (!g_fs_ok) {
        return;
    }
    eepfs_erase(SAVE_PATH);
    save_defaults();
    save_store();
    debugf("L38: wiped save\n");
}

int main(void)
{
    debug_init_isviewer();
    debug_init_usblog();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    rdpq_init();
    joypad_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));

    eeprom_type_t et = eeprom_present();
    debugf("L38: eeprom_present=%d blocks=%d\n", (int)et, eeprom_total_blocks());

    if (et != EEPROM_NONE) {
        int ir = eepfs_init(FS_FILES, 1);
        if (ir == EEPFS_ESUCCESS) {
            g_fs_ok = true;
            if (!eepfs_verify_signature()) {
                debugf("L38: bad FS signature — wipe\n");
                eepfs_wipe();
            }
            save_load();
        } else {
            debugf("L38: eepfs_init failed %d\n", ir);
        }
    } else {
        debugf("L38: no EEPROM (emulator may need savetype header / settings)\n");
    }

    bool running = false;
    bool paused = false;
    float run_t = 0.f;
    char line[80];
    const char *msg = "A=start run";

    uint64_t last_ms = get_ticks_ms();

    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        uint64_t now_ms = get_ticks_ms();
        float dt = (float)(now_ms - last_ms) / 1000.f;
        last_ms = now_ms;
        if (dt < 0.f) {
            dt = 0.f;
        }
        if (dt > 0.1f) {
            dt = 0.1f;
        }

        if (pressed.z) {
            save_wipe();
            msg = "Save wiped";
            running = false;
            paused = false;
            run_t = 0.f;
        }

        if (pressed.start && running) {
            paused = !paused;
            msg = paused ? "Paused" : "Running…";
        }

        if (pressed.a) {
            if (!running) {
                running = true;
                paused = false;
                run_t = 0.f;
                msg = "Running… START=pause  A=finish";
            } else if (!paused) {
                /* Finish run */
                running = false;
                g_save.runs++;
                bool record = (run_t < g_save.best_time);
                if (record) {
                    g_save.best_time = run_t;
                    save_store();
                    msg = "NEW BEST — saved!";
                } else {
                    save_store(); /* still bump runs */
                    msg = "Finished (not a best)";
                }
            }
        }

        if (running && !paused) {
            run_t += dt;
        }

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 18, .g = 40, .b = 36, .a = 255 });

        rdpq_text_print(NULL, 1, 12, 14, "L38 — Save & high scores");
        snprintf(line, sizeof(line), "EEPROM: %s  fs=%s",
                 et == EEPROM_NONE ? "NONE" :
                 et == EEPROM_4K ? "4k" : "16k",
                 g_fs_ok ? "ok" : "off");
        rdpq_text_print(NULL, 1, 12, 36, line);

        if (g_save.best_time < 1e8f) {
            snprintf(line, sizeof(line), "Best: %.2fs  runs: %lu",
                     (double)g_save.best_time, (unsigned long)g_save.runs);
        } else {
            snprintf(line, sizeof(line), "Best: --  runs: %lu",
                     (unsigned long)g_save.runs);
        }
        rdpq_text_print(NULL, 1, 12, 60, line);

        snprintf(line, sizeof(line), "This run: %.2fs%s",
                 (double)run_t, paused ? " (paused)" : "");
        rdpq_text_print(NULL, 1, 12, 84, line);
        rdpq_text_print(NULL, 1, 12, 108, msg);
        rdpq_text_print(NULL, 1, 12, 140, "A start/finish  START pause  Z wipe");
        rdpq_text_print(NULL, 1, 12, 168, "Power-cycle: best should remain");
        rdpq_text_print(NULL, 1, 12, 200, "Capstone uses the same eepfs pattern");

        rdpq_detach_show();
    }
}
