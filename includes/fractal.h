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

# define WIDTH 1920
# define HEIGHT 1080

#define LOCATION_MENU 0
#define LOCATION_TRIANGLE 1
#define LOCATION_RECTANGLE 2
#define LOCATION_MANDELBROT 3
#define LOCATION_JULIA 4
#define LOCATION_EXIT 5

extern int location;

extern double g_zoom;
extern int g_needs_clear;
extern double shift_y;
extern double shift_x;
extern double escape_value;
extern int max_iteration;
extern bool need_redraw;
extern bool g_drag_active;
extern double g_drag_start_x;
extern double g_drag_start_y;
extern double deltatime;
extern pthread_mutex_t start;

extern int move_left;
extern int move_right;
extern int move_down;
extern int move_up;



# include "structs.h"
# include "utils.h"
# include "2dfractal.h"
# include "callback.h"
# include "gui.h"
# include "movement.h"
# include "zoom.h"

extern shape Shape;

void render_loop(void);

#endif


