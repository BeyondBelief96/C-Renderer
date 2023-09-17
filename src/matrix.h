#ifndef MATRIX_H
#define MATRIX_H
#include "vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

#endif

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);
vec4_t mat4_mult_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mult_mat4(mat4_t m, mat4_t n);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation(float ax, float ay, float az);
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);