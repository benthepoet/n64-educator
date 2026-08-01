/**
 * ng_math — small course math helpers for N64 Educator
 * =====================================================
 *
 * WHAT YOU GET
 * ------------
 * vec2 / vec3 helpers (add, scale, length, normalize, cross)
 * mat4 helpers (identity, multiply, translate/rotate/scale, look_at, perspective)
 *
 * CONVENTIONS (see docs/reference/conventions.md)
 * -----------------------------------------------
 *   +Y is "up" in world diagrams
 *   Matrices are **column-major** (m[col*4 + row])
 *   Transform a point as **M * v** (matrix times vector)
 *   Angles for these helpers are in **radians**
 *     (use ng_deg_to_rad / ng_rad_to_deg at the bottom)
 *
 * HOW BEGINNERS SHOULD USE THIS
 * -----------------------------
 * You do NOT need to derive every formula. Call the helpers, print results,
 * and match them to the Module 1 ROMs. Read function bodies when curious.
 *
 * Intentionally simple — clarity over micro-optimizations.
 */
#ifndef NG_MATH_H
#define NG_MATH_H

#include <math.h>
#include <stdbool.h>

#ifndef NG_PI
#define NG_PI 3.14159265358979323846f
#endif

typedef struct {
    float x, y;
} ng_vec2;

typedef struct {
    float x, y, z;
} ng_vec3;

/* Column-major 4x4: m[col * 4 + row] */
typedef struct {
    float m[16];
} ng_mat4;

/* ---- vec2 ---- */
static inline ng_vec2 ng_v2(float x, float y) { return (ng_vec2){ x, y }; }
static inline ng_vec2 ng_v2_add(ng_vec2 a, ng_vec2 b) { return ng_v2(a.x + b.x, a.y + b.y); }
static inline ng_vec2 ng_v2_sub(ng_vec2 a, ng_vec2 b) { return ng_v2(a.x - b.x, a.y - b.y); }
static inline ng_vec2 ng_v2_scale(ng_vec2 a, float s) { return ng_v2(a.x * s, a.y * s); }
static inline float ng_v2_dot(ng_vec2 a, ng_vec2 b) { return a.x * b.x + a.y * b.y; }
static inline float ng_v2_len(ng_vec2 a) { return sqrtf(ng_v2_dot(a, a)); }
static inline ng_vec2 ng_v2_normalize(ng_vec2 a)
{
    float len = ng_v2_len(a);
    if (len < 1e-6f) {
        return ng_v2(0.f, 0.f);
    }
    return ng_v2_scale(a, 1.f / len);
}

/* ---- vec3 ---- */
static inline ng_vec3 ng_v3(float x, float y, float z) { return (ng_vec3){ x, y, z }; }
static inline ng_vec3 ng_v3_add(ng_vec3 a, ng_vec3 b) { return ng_v3(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline ng_vec3 ng_v3_sub(ng_vec3 a, ng_vec3 b) { return ng_v3(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline ng_vec3 ng_v3_scale(ng_vec3 a, float s) { return ng_v3(a.x * s, a.y * s, a.z * s); }
static inline float ng_v3_dot(ng_vec3 a, ng_vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float ng_v3_len(ng_vec3 a) { return sqrtf(ng_v3_dot(a, a)); }
static inline ng_vec3 ng_v3_normalize(ng_vec3 a)
{
    float len = ng_v3_len(a);
    if (len < 1e-6f) {
        return ng_v3(0.f, 0.f, 0.f);
    }
    return ng_v3_scale(a, 1.f / len);
}
static inline ng_vec3 ng_v3_cross(ng_vec3 a, ng_vec3 b)
{
    return ng_v3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

/* ---- mat4 ---- */
void ng_mat4_identity(ng_mat4 *out);
void ng_mat4_mul(ng_mat4 *out, const ng_mat4 *a, const ng_mat4 *b);
ng_vec3 ng_mat4_mul_point(const ng_mat4 *m, ng_vec3 p);
ng_vec3 ng_mat4_mul_vector(const ng_mat4 *m, ng_vec3 v);

void ng_mat4_translation(ng_mat4 *out, float x, float y, float z);
void ng_mat4_scale(ng_mat4 *out, float x, float y, float z);
void ng_mat4_rotation_z(ng_mat4 *out, float radians);
void ng_mat4_rotation_y(ng_mat4 *out, float radians);
void ng_mat4_rotation_x(ng_mat4 *out, float radians);

/** Build TRS as T * R_z * S (course default for 2D-ish demos). */
void ng_mat4_trs_z(ng_mat4 *out, ng_vec3 t, float rot_z_rad, ng_vec3 s);

/** Look-at view matrix (camera at eye, looking at target, +Y up). */
void ng_mat4_look_at(ng_mat4 *out, ng_vec3 eye, ng_vec3 target, ng_vec3 up);

/** Perspective projection (FOV in radians, aspect = width/height). */
void ng_mat4_perspective(ng_mat4 *out, float fovy_rad, float aspect, float znear, float zfar);

static inline float ng_deg_to_rad(float d) { return d * (NG_PI / 180.f); }
static inline float ng_rad_to_deg(float r) { return r * (180.f / NG_PI); }

#endif /* NG_MATH_H */
