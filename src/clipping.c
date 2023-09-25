#include <math.h>
#include "clipping.h"

#define NUM_PLANES 6

plane_t frustum_planes[NUM_PLANES];

///////////////////////////////////////////////////////////////////////////////
// Frustum planes are defined by a point and a normal vector
///////////////////////////////////////////////////////////////////////////////
// Near plane   :  P=(0, 0, znear), N=(0, 0,  1)
// Far plane    :  P=(0, 0, zfar),  N=(0, 0, -1)
// Top plane    :  P=(0, 0, 0),     N=(0, -cos(fov/2), sin(fov/2))
// Bottom plane :  P=(0, 0, 0),     N=(0, cos(fov/2), sin(fov/2))
// Left plane   :  P=(0, 0, 0),     N=(cos(fov/2), 0, sin(fov/2))
// Right plane  :  P=(0, 0, 0),     N=(-cos(fov/2), 0, sin(fov/2))
///////////////////////////////////////////////////////////////////////////////
//
//           /|\
//         /  | | 
//       /\   | |
//     /      | |
//  P*|-->  <-|*|   ----> +z-axis
//     \      | |
//       \/   | |
//         \  | | 
//           \|/
//
///////////////////////////////////////////////////////////////////////////////
void init_frustum_planes(float fov, float z_near, float z_far) {
	float cos_half_fov = cos(fov / 2);
	float sin_half_fov = sin(fov / 2);

	frustum_planes[LEFT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.x = cos_half_fov;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[LEFT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.x = -cos_half_fov;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[RIGHT_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustum_planes[TOP_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[TOP_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.y = -cos_half_fov;
	frustum_planes[TOP_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustum_planes[BOTTOM_FRUSTUM_PLANE].point = vec3_new(0, 0, 0);
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.y = cos_half_fov;
	frustum_planes[BOTTOM_FRUSTUM_PLANE].normal.z = sin_half_fov;

	frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[NEAR_FRUSTUM_PLANE].normal.z = 1;

	frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
	frustum_planes[FAR_FRUSTUM_PLANE].normal.x = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.y = 0;
	frustum_planes[FAR_FRUSTUM_PLANE].normal.z = -1;
}

polygon_t create_polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2) {
	polygon_t polygon = {
		.vertices = {
			v0, v1, v2
		},
		.num_vertices = 3
	};

	return polygon;
}

void clip_polygon(polygon_t* polygon) {
	clip_polygon_against_plane(polygon, frustum_planes[LEFT_FRUSTUM_PLANE]);
	clip_polygon_against_plane(polygon, frustum_planes[RIGHT_FRUSTUM_PLANE]);
	clip_polygon_against_plane(polygon, frustum_planes[TOP_FRUSTUM_PLANE]);
	clip_polygon_against_plane(polygon, frustum_planes[BOTTOM_FRUSTUM_PLANE]);
	clip_polygon_against_plane(polygon, frustum_planes[NEAR_FRUSTUM_PLANE]);
	clip_polygon_against_plane(polygon, frustum_planes[FAR_FRUSTUM_PLANE]);
}

void clip_polygon_against_plane(polygon_t* polygon, plane_t plane) {
	vec3_t plane_point = plane.point;
	vec3_t plane_normal = plane.normal;

	// The array of inside vertices that will be a part of the final polygon returned.
	vec3_t inside_vertices[MAX_NUM_POLY_VERTICES];
	int num_inside_vertices = 0;

	// Start current and previous vertex with the first and last polygon vertices
	vec3_t* current_vertex = &polygon->vertices[0];
	vec3_t* previous_vertex = &polygon->vertices[polygon->num_vertices - 1];
	float current_dot = 0;
	float previous_dot = vec3_dot(vec3_sub(*previous_vertex, plane_point), plane_normal);

	while(current_vertex != &polygon->vertices[polygon->num_vertices]) {
		current_dot = vec3_dot(vec3_sub(*current_vertex, plane_point), plane_normal);

		// If we changed from inside to outside or vice-versa
		if(current_dot * previous_dot < 0) {
			// Calculate the interpolation factor t = dotQ1 / (dotQ1 - dotQ2)
			float interpolation_factor = previous_dot / (previous_dot - current_dot);

			// Calculate the new intersection point in the list of "inside vertices" I = Q1 + t*(Q2 - Q1)
			vec3_t intersection_point = vec3_clone(current_vertex);
			intersection_point = vec3_sub(intersection_point, *previous_vertex);
			intersection_point = vec3_mul(intersection_point, interpolation_factor);
			intersection_point = vec3_add(intersection_point, *previous_vertex);
			// insert current vertex in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = vec3_clone(&intersection_point);
			num_inside_vertices++;
		}

		// If current point is inside the plane
		if(current_dot > 0) {
			// insert current vertex in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = vec3_clone(current_vertex);
			num_inside_vertices++;
		}

		// Move to the next vertex
		previous_dot = current_dot;
		previous_vertex = current_vertex;
		current_vertex++;
	}

	// At the end, copy the list of inside vertices into the destination polygon (out parameter)
	for(int i = 0; i < num_inside_vertices; i++) {
		polygon->vertices[i] = vec3_clone(&inside_vertices[i]);
	}

	polygon->num_vertices = num_inside_vertices;
}