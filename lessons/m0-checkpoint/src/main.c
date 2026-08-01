/**
 * Module 0 checkpoint
 * ============================================================================
 * Combines L01–L04: loop, joypad edges, DFS sprite.
 * A = +1 count (pressed, not held), B = reset, stars drawn for count.
 * If this ROM works, your foundations are solid.
 * DOCS: docs/guide/m0/checkpoint.md
 */
\n

#include <libdragon.h>
#include <stdio.h>

int main(void)
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE,
                 FILTERS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();
    rdpq_text_register_font(1, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR));
    joypad_init();

    sprite_t *star = sprite_load("rom:/star.sprite");
    int count = 0;
    char line[48];

    while (1) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

        if (pressed.a) {
            count++;
        }
        if (pressed.b) {
            count = 0;
        }

        snprintf(line, sizeof(line), "Count: %d", count);

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 10, .g = 28, .b = 24, .a = 255 });

        rdpq_text_print(NULL, 1, 24, 30, "Module 0 checkpoint");
        rdpq_text_print(NULL, 1, 24, 56, "A = +1   B = reset");
        rdpq_text_print(NULL, 1, 24, 90, line);

        if (star) {
            rdpq_set_mode_standard();
            rdpq_mode_alphacompare(1);
            /* Draw one star per count (capped so we do not fill the screen). */
            int n = count < 12 ? count : 12;
            for (int i = 0; i < n; i++) {
                float x = 24.0f + (float)(i % 6) * 40.0f;
                float y = 140.0f + (float)(i / 6) * 40.0f;
                rdpq_sprite_blit(star, x, y, NULL);
            }
        }

        rdpq_text_print(NULL, 1, 24, 220, "If this works, foundations are solid.");

        rdpq_detach_show();
    }
}
