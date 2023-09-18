
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "matrix.h"

float M_PI = 3.14159265358979323846;

// Array of triangles that should be rendered frame by frame.
triangle_t* triangles_to_render = NULL;
// Global Variables for execution status and game.
vec3_t camera_position = { 0, 0, 0 };
mat4_t proj_matrix;
float fov_factor = 640;  
bool is_running = false;
int previous_frame_time = 0;

//Function that recieves a 3D vector and returns a projected 2D point.
//This uses a perspective perspection by dividing by the original points depth.
vec2_t project(vec3_t point)
{
	vec2_t projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};

	return projected_point;
} 

void setup(void)
{
	//Initialize render mode and triangle culling method.
	cull_method = CULL_BACKFACE;
	render_method = RENDER_WIRE_VERTEX;

	//Allocate the required memory in bytes to hold color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	//Create SDL color buffer texture to display color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	//Iniitialize the perspective projection matrix
	float fov = 60.0 * (M_PI / 180);
	float aspect = (float) window_height / window_width;
	float znear = 0.1;
	float zfar = 100.0;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

	//Loads the cube values in our global mesh.
	load_cube_mesh_data();
	//load_obj_file_data("./assets/f22.obj");
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		if (event.key.keysym.sym == SDLK_1) 
			render_method = RENDER_WIRE_VERTEX;
		if (event.key.keysym.sym == SDLK_2) 
			render_method = RENDER_WIRE;
		if (event.key.keysym.sym == SDLK_3) 
			render_method = RENDER_FILL_TRIANGLE;
		if (event.key.keysym.sym == SDLK_4) 
			render_method = RENDER_FILL_TRIANGLE_WIRE;
		if (event.key.keysym.sym == SDLK_c) 
			cull_method = CULL_BACKFACE;
		if (event.key.keysym.sym == SDLK_d) 
			cull_method = CULL_NONE;
		break;
	}
}

void update(void)
{
	//Wait some time until we reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	//Only delay execution if we are running too fast.
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);

	previous_frame_time = SDL_GetTicks64();

	//Initialize the array of triangles to render
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	// mesh.rotation.y += 0.01;
	// mesh.rotation.z += 0.01;
	//mesh.scale.x += 0.002;
	// mesh.scale.y += 0.002;
	// mesh.scale.z += 0.002;
	// mesh.translation.x += 0.01;
	mesh.translation.z = 5;

	//Create a scale matrix that will be used to multiply mesh vertices.
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix = mat4_make_rotation(mesh.rotation.x, mesh.rotation.y, mesh.rotation.z);
	int num_faces = array_length(mesh.faces);
	//Loop all triangle faces of our mesh.
	for (int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		vec4_t transformed_vertices[3];
		//Loop all 3 vertices and apply transformations
		for (int j = 0; j < 3; j++)
		{
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);
			//Create a World Matrix combining scale, rotation, and translation matrices.
			mat4_t world_matrix = mat4_identity();
			//Multiply all matrices and load the world matrix.
			world_matrix = mat4_mult_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mult_mat4(rotation_matrix, world_matrix);
			world_matrix = mat4_mult_mat4(translation_matrix, world_matrix);
			//Multiply the world matrix by the original vertex.
			transformed_vertex = mat4_mult_vec4(world_matrix, transformed_vertex);

			//Save transformed vertex in the array of transformed vertices.
			transformed_vertices[j] = transformed_vertex;
		}

		if (cull_method == CULL_BACKFACE) {
			//Back face culling
			vec3_t a = vec3_from_vec4(transformed_vertices[0])/*   A   */;
			vec3_t b = vec3_from_vec4(transformed_vertices[1])/*  | \*/;
			vec3_t c = vec3_from_vec4(transformed_vertices[2])/* C---B*/;
			//1. Find B-A and C-A
			vec3_t vec_ab = subtract_vec3(b, a);
			vec3_t vec_ac = subtract_vec3(c, a);
			vec3_normalize(&vec_ab);
			vec3_normalize(&vec_ac);

			//2 Find normal vector to face
			vec3_t normal = vec3_cross(vec_ab, vec_ac);
			vec3_normalize(&normal);
			//Find the camera ray
			vec3_t camera_ray = subtract_vec3(camera_position, a);
			//Dot normal and camera_ray and figure out if triangle is facing camera.
			float dot_camera_normal = vec3_dot(camera_ray, normal);

			//Bypass the triangles that are looking away from the camera.
			if (dot_camera_normal < 0) {
				continue;
			}
		}

		vec4_t projected_points[3];
		//Loop all transformed vertices and project transformed point into screen space.
		for (int j = 0; j < 3; j++)
		{
			//Project transformed vertex into screen space.
			projected_points[j] = mat4_mult_vec4_project(proj_matrix, transformed_vertices[j]);

			//Scale into the view port
			projected_points[j].x *= (window_width / 2.0);
			projected_points[j].y *= (window_height / 2.0);

			//Translate projected point to middle of the screen.
			projected_points[j].x += window_width / 2.0;
			projected_points[j].y += window_height / 2.0;
		}

		//Calculate the average depth of each face based on the vertices 
		//after transformation (Painters Algorithm)
		float avg_depth = (transformed_vertices[0].z +
		 transformed_vertices[1].z +
		  transformed_vertices[2].z) / 3.0;
		
		//store projected point into our projected triangle structure.
		triangle_t projected_triangle = {
			.points = {
				{projected_points[0].x, projected_points[0].y},
				{projected_points[1].x, projected_points[1].y},
				{projected_points[2].x, projected_points[2].y},
			},
			.color = mesh_face.color,
			.avg_depth = avg_depth
		};

		//Each iteration store the projected triangle that was created.
		array_push(triangles_to_render, projected_triangle);
	}	

	//Sort the triangle to render by their average depth.
	int num_triangles = array_length(triangles_to_render);

	//Bubble Sort
	for(int i = 0; i < num_triangles; i++) {
		for(int j = 0; j < num_triangles; j++) {
			if(triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth) {
				triangle_t temp = triangles_to_render[i];
				triangles_to_render[i] = triangles_to_render[j];
				triangles_to_render[j] = temp;
			}
		}
	}
}

void render(void)
{
	draw_grid(10);

	//Loop all projected triangles and render them.
	int num_triangles = array_length(triangles_to_render);
	for (int i = 0; i < num_triangles; i++)
	{
		triangle_t triangle = triangles_to_render[i];
		if (render_method == RENDER_WIRE_VERTEX) {
			//Draw vertex points
			draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFF0000);
			draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFF0000);
			draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFF0000);
		}
		if (render_method == RENDER_WIRE ||
		 	render_method == RENDER_FILL_TRIANGLE_WIRE ||
		  	render_method == RENDER_WIRE_VERTEX) {
			// Draw unfilled triangles
			draw_triangle(triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			// Draw filled triangles
			draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				triangle.color);
		}
	}

	//Clear the array of triangles to render every frame loop
	array_free(triangles_to_render);

	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
}

//Free the memory that was dynamically allocated by the program.
void free_resources(void)
{
	free(color_buffer);
	array_free(mesh.vertices);
	array_free(mesh.faces);
}

int main(void)
{
	/* create SDL window */
	is_running = initialize_window();
	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	free_resources();

	return 0;
}