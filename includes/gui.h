#ifndef GUI_H
# define GUI_H

#include "fractal.h"

void main_menu(void);
int point_in_button(double x, double y, button rect);
void draw_button(button rect, double red, double green, double blue);
void write_button_text(button rect, const char *label);
void draw_slider(button rect, double r, double g, double b, int x, int y);

#endif