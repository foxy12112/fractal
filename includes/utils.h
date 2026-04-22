#ifndef UTILS_H
# define UTILS_H

# include "fractal.h"

double scale(double unscaled, double new_min, double new_max, double old_max);
point sum_complex(point a, point b);
point square_complex(point a);
color colorize(int col);

#endif