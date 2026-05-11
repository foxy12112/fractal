#include "fractal.h"

int location = LOCATION_MENU;
double g_zoom = 1.0f;
int g_needs_clear = 0;
double shift_y = 0;
double shift_x = 0;
double escape_value = 4.0;
int max_iteration = 200;
bool need_redraw = false;
bool g_drag_active = false;
double g_drag_start_x = 0.0;
double g_drag_start_y = 0.0;
double deltatime = 0;
pthread_mutex_t start;

int move_left = 0;
int move_right = 0;
int move_down = 0;
int move_up = 0;

shape Shape;