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

void draw_rect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//(width * row) + column
			int current_x = x + i;
			int current_y = y + j;
			draw_pixel(current_x, current_y, color);
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if(x >= 0 && x < window_width && y >= 0 && y < window_height)
	{
		color_buffer[(window_width * y) + x] = color;
	}
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_line_dda(x0, y0, x1, y1, color);
	draw_line_dda(x1, y1, x2, y2, color);
	draw_line_dda(x2, y2, x0, y0, color);
}

void draw_line_dda(int x0, int y0, int x1, int y1, uint32_t color)
{
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;
	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);
	
	float x_inc = delta_x / (float) side_length;
	float y_inc = delta_y / (float) side_length; //Slope of the line (delta y /delta x )

	float current_x = x0;
	float current_y = y0;

	for(int i = 0; i <= side_length; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}