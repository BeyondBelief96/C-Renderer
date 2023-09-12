#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool initialize_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	//Use SDL to query monitor width/height.
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	window_width = display_mode.w * 0.8;
	window_height = display_mode.h * 0.8;

	window = SDL_CreateWindow("3DRenderer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_RESIZABLE);

	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

void render_color_buffer(void)
{
	SDL_UpdateTexture(color_buffer_texture, NULL,
		color_buffer, sizeof(uint32_t) * window_width);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw_grid(int grid_spacing)
{
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			if (x % grid_spacing == 0 || y % grid_spacing == 0)
				color_buffer[(window_width * y) + x] = 0xFF333333;
		}
	}
}


void draw_dotted_grid(int grid_spacing)
{
	for (int y = 0; y < window_height; y += grid_spacing)
	{
		for (int x = 0; x < window_width; x+= grid_spacing)
		{
			color_buffer[(window_width * y) + x] = 0xFF333333;
		}
	}
}

//the x, y position is the position of the top left corner of the rectangle
void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//(width * row) + column
			int current_x = x + i;
			int current_y = y + j;
			color_buffer[(window_width * current_y) + current_x] = color;
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if(x < window_width && y < window_height)
	{
		color_buffer[(window_width * y) + x] = color;
	}
}