/**
 * ng_game.h — tiny shared helpers for Module 4+ gameplay lessons
 * =================================================================
 *
 * WHY THIS FILE EXISTS
 * --------------------
 * Every gameplay ROM needs the same boring utilities:
 *   - ignore stick noise near the center (deadzone)
 *   - measure seconds for delta-time movement
 *   - smooth blend numbers and angles
 *   - clamp values into a range
 *
 * Keeping them here means lessons stay focused on *game ideas*, not copy-paste
 * math bugs. All functions are static inline — no .c file to link.
 *
 * Include after libdragon if you want, but only math.h is required beyond that.
 */

#ifndef NG_GAME_H
#define NG_GAME_H

#include <libdragon.h>
#include <math.h>

/** Stick axes near 0 are treated as 0 (see Module 0 controllers). */
#define NG_DEADZONE 10

#ifndef NG_PI
#define NG_PI 3.14159265358979323846f
#endif

/**
 * Deadzone: return 0 if v is "close enough" to center, else v unchanged.
 * Use on raw joypad stick_x / stick_y before building a move vector.
 */
static inline int ng_dz(int v)
{
    return (v > -NG_DEADZONE && v < NG_DEADZONE) ? 0 : v;
}

/**
 * Seconds since boot as a float (from the CPU timer).
 * Subtract two samples to get delta time for one frame:
 *   float dt = ng_time_s() - last; last = ng_time_s();
 */
static inline float ng_time_s(void)
{
    return (float)((double)get_ticks_us() / 1000000.0);
}

/** Linear interpolate: a when t=0, b when t=1, mix in between. */
static inline float ng_lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

/**
 * Interpolate angles in radians the short way around the circle.
 * Plain lerp(6.0, 0.1) would spin the long way; this unwraps the difference
 * into (-pi, pi] first — essential for smooth character turning.
 */
static inline float ng_lerp_angle(float a, float b, float t)
{
    float diff = b - a;
    while (diff > NG_PI) {
        diff -= 2.f * NG_PI;
    }
    while (diff < -NG_PI) {
        diff += 2.f * NG_PI;
    }
    return a + diff * t;
}

/** Keep v inside [lo, hi]. */
static inline float ng_clamp(float v, float lo, float hi)
{
    if (v < lo) {
        return lo;
    }
    if (v > hi) {
        return hi;
    }
    return v;
}

#endif /* NG_GAME_H */
