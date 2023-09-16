#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

// 2D VECTOR FUNCTIONS
float vec2_length(vec2_t v);
vec2_t add_vec2(vec2_t a, vec2_t b);
vec2_t subtract_vec2(vec2_t a, vec2_t b);
vec2_t vec2_scale(vec2_t v, float factor);
vec2_t vec2_divide(vec2_t v, float factor);
float vec2_dot(vec2_t a, vec2_t b);
float vec2_cross_area(vec2_t a, vec2_t b);
vec2_t vec2_rotate_z(vec2_t v, float angle);

// 3D VECTOR FUNCTIONS
float vec3_length(vec3_t v);
vec3_t add_vec3(vec3_t a, vec3_t b);
vec3_t subtract_vec3(vec3_t a, vec3_t b);
vec3_t vec3_scale(vec3_t v, float factor);
vec3_t vec3_divide(vec3_t v, float factor);
float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);
// Rotates the vector about the x-axis.
vec3_t vec3_rotate_x(vec3_t v, float angle);

//Rotates the vector about the y-axis.
vec3_t vec3_rotate_y(vec3_t v, float angle);

//Rotates the vector about the z-axis.
vec3_t vec3_rotate_z(vec3_t v, float angle);
#endif