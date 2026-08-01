#include "ng_math.h"

void ng_mat4_identity(ng_mat4 *out)
{
    for (int i = 0; i < 16; i++) {
        out->m[i] = 0.f;
    }
    out->m[0] = out->m[5] = out->m[10] = out->m[15] = 1.f;
}

void ng_mat4_mul(ng_mat4 *out, const ng_mat4 *a, const ng_mat4 *b)
{
    ng_mat4 r;
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            r.m[col * 4 + row] =
                a->m[0 * 4 + row] * b->m[col * 4 + 0] +
                a->m[1 * 4 + row] * b->m[col * 4 + 1] +
                a->m[2 * 4 + row] * b->m[col * 4 + 2] +
                a->m[3 * 4 + row] * b->m[col * 4 + 3];
        }
    }
    *out = r;
}

ng_vec3 ng_mat4_mul_point(const ng_mat4 *m, ng_vec3 p)
{
    float x = m->m[0] * p.x + m->m[4] * p.y + m->m[8] * p.z + m->m[12];
    float y = m->m[1] * p.x + m->m[5] * p.y + m->m[9] * p.z + m->m[13];
    float z = m->m[2] * p.x + m->m[6] * p.y + m->m[10] * p.z + m->m[14];
    float w = m->m[3] * p.x + m->m[7] * p.y + m->m[11] * p.z + m->m[15];
    if (fabsf(w) > 1e-6f) {
        float inv = 1.f / w;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    return ng_v3(x, y, z);
}

ng_vec3 ng_mat4_mul_vector(const ng_mat4 *m, ng_vec3 v)
{
    /* Ignore translation column */
    return ng_v3(
        m->m[0] * v.x + m->m[4] * v.y + m->m[8] * v.z,
        m->m[1] * v.x + m->m[5] * v.y + m->m[9] * v.z,
        m->m[2] * v.x + m->m[6] * v.y + m->m[10] * v.z);
}

void ng_mat4_translation(ng_mat4 *out, float x, float y, float z)
{
    ng_mat4_identity(out);
    out->m[12] = x;
    out->m[13] = y;
    out->m[14] = z;
}

void ng_mat4_scale(ng_mat4 *out, float x, float y, float z)
{
    ng_mat4_identity(out);
    out->m[0] = x;
    out->m[5] = y;
    out->m[10] = z;
}

void ng_mat4_rotation_z(ng_mat4 *out, float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);
    ng_mat4_identity(out);
    out->m[0] = c;
    out->m[1] = s;
    out->m[4] = -s;
    out->m[5] = c;
}

void ng_mat4_rotation_y(ng_mat4 *out, float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);
    ng_mat4_identity(out);
    out->m[0] = c;
    out->m[2] = -s;
    out->m[8] = s;
    out->m[10] = c;
}

void ng_mat4_rotation_x(ng_mat4 *out, float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);
    ng_mat4_identity(out);
    out->m[5] = c;
    out->m[6] = s;
    out->m[9] = -s;
    out->m[10] = c;
}

void ng_mat4_trs_z(ng_mat4 *out, ng_vec3 t, float rot_z_rad, ng_vec3 s)
{
    ng_mat4 T, R, S, RS;
    ng_mat4_translation(&T, t.x, t.y, t.z);
    ng_mat4_rotation_z(&R, rot_z_rad);
    ng_mat4_scale(&S, s.x, s.y, s.z);
    ng_mat4_mul(&RS, &R, &S);
    ng_mat4_mul(out, &T, &RS);
}

void ng_mat4_look_at(ng_mat4 *out, ng_vec3 eye, ng_vec3 target, ng_vec3 up)
{
    ng_vec3 f = ng_v3_normalize(ng_v3_sub(target, eye));
    ng_vec3 s = ng_v3_normalize(ng_v3_cross(f, up));
    ng_vec3 u = ng_v3_cross(s, f);

    ng_mat4_identity(out);
    out->m[0] = s.x;
    out->m[4] = s.y;
    out->m[8] = s.z;
    out->m[1] = u.x;
    out->m[5] = u.y;
    out->m[9] = u.z;
    out->m[2] = -f.x;
    out->m[6] = -f.y;
    out->m[10] = -f.z;
    out->m[12] = -ng_v3_dot(s, eye);
    out->m[13] = -ng_v3_dot(u, eye);
    out->m[14] = ng_v3_dot(f, eye);
}

void ng_mat4_perspective(ng_mat4 *out, float fovy_rad, float aspect, float znear, float zfar)
{
    float f = 1.f / tanf(fovy_rad * 0.5f);
    for (int i = 0; i < 16; i++) {
        out->m[i] = 0.f;
    }
    out->m[0] = f / aspect;
    out->m[5] = f;
    out->m[10] = (zfar + znear) / (znear - zfar);
    out->m[11] = -1.f;
    out->m[14] = (2.f * zfar * znear) / (znear - zfar);
}
