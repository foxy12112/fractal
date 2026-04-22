#ifndef MAIN_H
# define MAIN_H

# include "fractal.h"

point	zoom_point(point p);
void	draw_outline(void);
void	main_menu(void);
void	draw_mandelbrot(int x, int y);
void	render_loop(void);
point	random_point_in_triangle(void);
void	make_fractal_triangle(void);
point	random_point_in_rectangle(void);
void	make_fractal_rectangle(void);

#endif