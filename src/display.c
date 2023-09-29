#include "display.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* color_buffer = NULL;
static float* z_buffer = NULL;

static SDL_Texture* color_buffer_texture = NULL;
static int window_width;
static int window_height;

enum cull_method cull_method = CULL_BACKFACE;
enum render_method render_method = RENDER_WIRE;

int get_window_width(void) {
    return window_width;
}

void set_window_width(int width) {
    window_width = width;
}

int get_window_height(void) {
    return window_height;
}

void set_window_height(int height) {
    window_height = height;
}

int get_cull_method(void) {
    return cull_method;
} 

void set_cull_method(int method) {
    cull_method = method;
}

int get_render_method(void) {
    return render_method;
}

void set_render_method(int method) {
    render_method = method;
}

bool should_render_filled_triangles(void) {
   return render_method == RENDER_FILL_TRIANGLE ||
    render_method == RENDER_FILL_TRIANGLE_WIRE;
}

bool should_render_textured_triangles(void) {
    return render_method == RENDER_TEXTURED ||
     render_method == RENDER_TEXTURED_WIRE;
}

bool should_render_triangle_wireframe(void) {
    return render_method == RENDER_WIRE ||
            render_method == RENDER_WIRE_VERTEX ||
            render_method == RENDER_FILL_TRIANGLE_WIRE ||
            render_method == RENDER_TEXTURED_WIRE;
}

bool should_render_wired_vertex(void) {
    return render_method == RENDER_WIRE_VERTEX;
}

float get_zbuffer_at(int x, int y) {
    if(x < 0 || x >= window_width || y < 0 || y >= window_height) {
        return 1.0;
    }
    return z_buffer[(window_width * y) + x];
}

void update_zbuffer_at(int x, int y, float value) {
    if(x < 0 || x >= window_width || y < 0 || y >= window_height) {
        return;
    }
    z_buffer[(window_width * y) + x] = value;
}

bool initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Set width and height of the SDL window with the max screen resolution
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    int fullscreen_width = display_mode.w;
    int fullscreen_height = display_mode.h;

    //This values are used across the entire application and for initilizing
    //my color buffer and z buffer. If you divide these by the same value, you
    //preserve aspect ratio and achieve a retro effect.
    window_width = fullscreen_width / 1;
    window_height = fullscreen_height / 1;

    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullscreen_width * 0.5,
        fullscreen_height * 0.5,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    // Create a SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    // Allocate the required memory in bytes to hold the color buffer and the z-buffer
    color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
    z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

    // Creating a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    return true;
}

void draw_grid(void) {
    for (int y = 0; y < window_height; y += 10) {
        for (int x = 0; x < window_width; x += 10) {
            color_buffer[(window_width * y) + x] = 0xFF444444;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
        return;
    }
    color_buffer[(window_width * y) + x] = color;
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = delta_x / (float)longest_side_length;
    float y_inc = delta_y / (float)longest_side_length;

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i <= longest_side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int)(window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);

    SDL_RenderPresent(renderer);
}

void clear_color_buffer(uint32_t color) {
    for (int i = 0; i < window_width * window_height; i++) {
            color_buffer[i] = color;
    }
}

void clear_z_buffer(void) {
    for (int i = 0; i < window_width * window_height; i++) {
        z_buffer[i] = 1.0; 
    }
}

void destroy_window(void) {
    free(color_buffer);
    free(z_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
