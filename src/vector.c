#include <math.h>
#include "vector.h"

float vec2_length(vec2_t v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

float vec3_length(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec2_t add_vec2(vec2_t a, vec2_t b)
{
	vec2_t v = {a.x + b.x, a.y + b.y};
	return v;
}
vec2_t subtract_vec2(vec2_t a, vec2_t b)
{
	vec2_t v = {a.x - b.x, a.y - b.y};
	return v;
}

vec3_t add_vec3(vec3_t a, vec3_t b)
{
	vec3_t v = {a.x + b.x, a.y + b.y, a.z + b.z};
	return v;
}
vec3_t subtract_vec3(vec3_t a, vec3_t b)
{
	vec3_t v = {a.x - b.x, a.y - b.y, a.z - b.z};
	return v;
}

vec2_t vec2_scale(vec2_t v, float factor)
{
	vec2_t scaled_vector = {v.x * factor, v.y * factor };
	return scaled_vector;
}
vec2_t vec2_divide(vec2_t v, float factor)
{
	vec2_t result = {v.x / factor, v.y / factor};
	return result;
}

vec3_t vec3_scale(vec3_t v, float factor)
{
	vec3_t result = {v.x * factor, v.y * factor, v.z * factor };
	return result;
}
vec3_t vec3_divide(vec3_t v, float factor)
{
	vec3_t result = {v.x / factor, v.y / factor, v.z / factor};
	return result;
}

float vec2_dot(vec2_t a, vec2_t b)
{
	float result = (a.x * b.x) + (a.y *b.y);
	return result;
}

float vec2_cross_area(vec2_t a, vec2_t b)
{
	float result = a.x * b.y - a.y*b.x;
	return result;
}

float vec3_dot(vec3_t a, vec3_t b)
{
	float result = (a.x * b.x) + (a.y *b.y) + (a.z * b.z);
	return result;
}

vec2_t vec2_rotate_z(vec2_t v, float angle)
{
	vec2_t rotated_vector =  {
		.x = v.x * cosf(angle) - v.y * sinf(angle),
		.y = v.x * sinf(angle) + v.y * cosf(angle),
	};

	return rotated_vector;
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
	vec3_t result = {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};

	return result;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
	vec3_t rotated_vector =  {
		.x = v.x * cosf(angle) - v.y * sinf(angle),
		.y = v.x * sinf(angle) + v.y * cosf(angle),
		.z = v.z
	};

	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
	vec3_t rotated_vector =  {
		.x = v.x * cosf(angle) - v.z * sinf(angle),
		.y = v.y,
		.z = v.x * sinf(angle) + v.z * cosf(angle),
	};

	return rotated_vector;
}

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
	vec3_t rotated_vector =  {
		.x = v.x,
		.y = v.y * cosf(angle) - v.z * sinf(angle),
		.z = v.y * sinf(angle) + v.z * cosf(angle),
	};

	return rotated_vector;
}