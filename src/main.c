#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

// Array of triangles that should be rendered frame by frame.
triangle_t* triangles_to_render = NULL;

// Global Variables for execution status and game.
vec3_t camera_position = { 0, 0, 0 };

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
	//Allocate the required memory in bytes to hold color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	//Create SDL color buffer texture to display color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	//Loads the cube values in our global mesh.
	load_obj_file_data("./assets/cube.obj");
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
		break;
	}
}

void update(void)
{
	//Wait some time until we reach the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	//Only delay execution if we are running too fast.
	if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		SDL_Delay(time_to_wait);
	
	previous_frame_time = SDL_GetTicks64();

	//Initialize the array of triangles to render
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;

	int num_faces = array_length(mesh.faces);
	//Loop all triangle faces of our mesh.
	for(int i = 0; i < num_faces; i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];


		vec3_t transformed_vertices[3];
		//Loop all 3 vertices and apply transformations
		for(int j = 0; j < 3; j++)
		{
			//Grab current vertex and transform vertex in world space.
			vec3_t transformed_vertex = face_vertices[j];
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			//Translate the vertex away from the camera
			transformed_vertex.z += -5;

			//Save transformed vertex in the array of transformed vertices.
			transformed_vertices[j] = transformed_vertex;
		}

		//Back face culling
		vec3_t a = transformed_vertices[0]/*   A   */;
		vec3_t b = transformed_vertices[1]/*  | \*/;
		vec3_t c = transformed_vertices[2]/* C---B*/;
		//1. Find B-A and C-A
		vec3_t vec_ab = subtract_vec3(b, a);
		vec3_t vec_ac = subtract_vec3(c, a);
		//2 Find normal vector to face
		vec3_t normal = vec3_cross(vec_ab, vec_ac);
		//Find the camera ray
		vec3_t camera_ray = subtract_vec3(camera_position, a);
		//Dot normal and camera_ray and figure out if triangle is facing camera.
		float dot_camera_normal = vec3_dot(camera_ray, normal);

		//Bypass the triangles that are looking away from the camera.
		if(dot_camera_normal < 0) {
			continue;
		}
			
		triangle_t projected_triangle;
		//Loop all transformed vertices and project transformed point into screen space.
		for(int j = 0; j < 3; j++) 
		{
			//Project transformed vertex into screen space.
			vec2_t projected_point = project(transformed_vertices[j]);

			//Scale and translate projected point to middle of the screen.
			projected_point.x += window_width / 2;
			projected_point.y += window_height / 2;

			//store projected point into our projected triangle structure.
			projected_triangle.points[j] = projected_point;
		}

		//Each iteration store the projected triangle that was created.
		array_push(triangles_to_render, projected_triangle);
	}
}

void render(void)
{
	draw_dotted_grid(10);

	//Loop all projected triangles and render them.
	int num_triangles = array_length(triangles_to_render);

	for (int i = 0; i < num_triangles; i++)
	{
		//Draw vertex points
		triangle_t triangle = triangles_to_render[i];
		draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
		draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);

		// Draw unfilled triangles
		draw_triangle(triangle.points[0].x, triangle.points[0].y,
		triangle.points[1].x, triangle.points[1].y,
		triangle.points[2].x, triangle.points[2].y,
		0xFF00FF00);
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