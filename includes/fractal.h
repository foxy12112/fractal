#ifndef FRACTAL_H
# define FRACTAL_H

# include "glad.h"
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <GLFW/glfw3.h>
# include <pthread.h>
# include <time.h>
# include <unistd.h>
# include <stdbool.h>

# define BLACK 0x000000
# define WHITE 0xFFFFFF

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

#endif


