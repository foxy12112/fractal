#include "utils.h"

double scale(double unscaled, double new_min, double new_max, double old_max)
{
	return (((new_max - new_min) * unscaled - 0) / (old_max - 0) + new_min);
}

point sum_complex(point a, point b)
{
	point result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

point square_complex(point a)
{
	point result;

	result.x = (a.x * a.x) - (a.y * a.y);
	result.y = 2 * a.x * a.y;
	return (result);
}

color colorize(int col)
{
	color a;
	a.r = ((col >> 16) & 0xFF) / 255.0f;
	a.g = ((col >> 8) & 0xFF) / 255.0f;
	a.b = (col & 0xFF) / 255.0f;
	return a;
}

double get_time()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + ts.tv_nsec / 1e9);
}
