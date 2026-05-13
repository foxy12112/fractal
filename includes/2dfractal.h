#ifndef DFRACTAL_H
# define DFRACTAL_H

#include "fractal.h"

void make_fractal_rectangle(void);
void make_fractal_triangle(void);
void *mandelbrot_thread(void *arg);
point random_point_in_rectangle(void);
point random_point_in_triangle(void);
void *simple_julia_thread(void *arg);


#endif