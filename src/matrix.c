#include "matrix.h"

mat4_t mat4_identity() {
    mat4_t mat4_identity = {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }};

        return mat4_identity;
    }

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    mat4_t identity = mat4_identity();
    identity.m[0][0] = sx;
    identity.m[1][1] = sy;
    identity.m[2][2] = sz;
    return identity;
}

vec4_t mat4_mult_mat4(mat4_t m, vec4_t v) {
    vec4_t result;
    result.x = m.m[0][0] * v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w;
    result.y = m.m[1][0] * v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w;
    result.z = m.m[2][0] * v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w;
    result.w = m.m[3][0] * v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w;
    return result;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    mat4_t result = mat4_identity();
    result.m[0][3] = tx;
    result.m[1][3] = ty;
    result.m[2][3] = tz;
    return result;
}