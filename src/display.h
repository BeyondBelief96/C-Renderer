#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

extern enum cull_method {
    CULL_NONE,
    CULL_BACKFACE
} cull_method;

extern enum render_method {
    RENDER_WIRE,
    RENDER_WIRE_VERTEX,
    RENDER_FILL_TRIANGLE,
    RENDER_FILL_TRIANGLE_WIRE,
    RENDER_TEXTURED,
    RENDER_TEXTURED_WIRE
} render_method;

bool initialize_window(void);

int get_window_width(void);
int get_window_height(void);
void set_window_width(int width);
void set_window_height(int height);

int get_cull_method(void);
int get_render_method(void);
void set_cull_method(int method);
void set_render_method(int method);

bool should_render_filled_triangles(void);
bool should_render_textured_triangles(void);
bool should_render_triangle_wireframe(void);
bool should_render_wired_vertex(void);

void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);

float get_zbuffer_at(int x, int y);
void update_zbuffer_at(int x, int y, float value);

void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void clear_z_buffer(void);

void destroy_window(void);

#endif
