#ifndef STRUCTS_H
# define STRUCTS_H

#include "fractal.h"

typedef struct s_point {
	double x;
	double y;
} point;

typedef struct s_color {
	double r;
	double g;
	double b;
	double a;
} color;

typedef struct s_pixel_data {
	color **col;
	int y_start;
	int y_end;
}	pixel_data;

typedef struct {
	double x;
	double y;
	double width;
	double height;
} button;

typedef struct {
	point A;
	point B;
	point C;
	point D;
	int points;
} shape;

typedef struct {
	button rect;
	int location;
	const char *label;
} menu_button;

static const menu_button g_menu_buttons[5] = {
	{{760.0f, 760.0f, 400.0f, 80.0f}, LOCATION_TRIANGLE, "TRIANGLE"},
	{{760.0f, 660.0f, 400.0f, 80.0f}, LOCATION_RECTANGLE, "RECTANGLE"},
	{{760.0f, 560.0f, 400.0f, 80.0f}, LOCATION_MANDELBROT, "MANDELBROT"},
	{{760.0f, 460.0f, 400.0f, 80.0f}, LOCATION_JULIA, "JULIA"},
	{{760.0f, 360.0f, 400.0f, 80.0f}, LOCATION_EXIT, "EXIT"}
};

#endif