/**
 * L04 — Assets on ROM (DFS)
 * ============================================================================
 * Assets live *inside* the .z64, not on your PC at runtime.
 *   PNG → mksprite → filesystem/*.sprite → packed DFS → rom:/name.sprite
 * Always dfs_init() then sprite_load("rom:/...").
 * DOCS: docs/guide/m0/l04-dfs.md
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

    /* Paths are on the *ROM* filesystem — not your PC's disk. */
    sprite_t *star = sprite_load("rom:/star.sprite");
    if (!star) {
        /* Should not happen if Makefile packed filesystem/star.sprite */
        while (1) {
            surface_t *disp = display_get();
            rdpq_attach(disp, NULL);
            rdpq_clear((color_t){ .r = 64, .g = 0, .b = 0, .a = 255 });
            rdpq_text_print(NULL, 1, 40, 100, "Failed to load rom:/star.sprite");
            rdpq_detach_show();
        }
    }

    float t = 0.0f;

    while (1) {
        t += 0.04f;
        /* Gentle bob for juice */
        float bob = fm_sinf(t) * 8.0f;
        float x = 144.0f;
        float y = 100.0f + bob;

        surface_t *disp = display_get();
        rdpq_attach(disp, NULL);
        rdpq_clear((color_t){ .r = 16, .g = 20, .b = 40, .a = 255 });

        rdpq_text_print(NULL, 1, 24, 28, "L04 — DFS assets");
        rdpq_text_print(NULL, 1, 24, 48, "Loaded rom:/star.sprite");

        rdpq_set_mode_standard();
        rdpq_mode_alphacompare(1);
        rdpq_sprite_blit(star, x, y, NULL);

        rdpq_text_print(NULL, 1, 24, 200, "PNG -> mksprite -> DFS -> sprite_load");

        rdpq_detach_show();
    }
}
