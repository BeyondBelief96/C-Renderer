#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

bool is_running = false;

void setup(void)
{
	//Allocate the required memory in bytes to hold color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	//Create SDL color buffer texture to display color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);
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
	//TODO
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	draw_grid(10);

	draw_pixel(20, 20, 0xFFFFFF00);
	draw_rect(300, 200, 300, 300, 0xFFFF00FF);


	render_color_buffer();
	clear_color_buffer(0x00000000);

	SDL_RenderPresent(renderer);
}

int main(void)
{
    /* create SDL window */
	is_running = initialize_window();
	setup();

	vec3_t vector = {2.0, 3.0, -4.0 };
	while (is_running)
	{
		process_input();
		update();
		render();
		
	}

	destroy_window();

	return 0;
}