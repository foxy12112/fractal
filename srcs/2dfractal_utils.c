#include "2dfractal.h"

point random_point_in_rectangle(void)
{
	point p;
	double min_x = Shape.A.x;
	double max_x = Shape.B.x;
	double min_y = Shape.A.y;
	double max_y = Shape.D.y;

	p.x = min_x + ((double)rand() / (double)RAND_MAX) * (max_x - min_x);
	p.y = min_y + ((double)rand() / (double)RAND_MAX) * (max_y - min_y);
	return (p);
}

point random_point_in_triangle(void)
{
	double r1 = (double)rand() / (double)RAND_MAX;
	double r2 = (double)rand() / (double)RAND_MAX;
	if (r1 + r2 > 1)
	{
		r1 = 1.0f - r1;
		r2 = 1.0f - r2;
	}
	point P;
	P.x = Shape.A.x + r1 * (Shape.B.x - Shape.A.x) + r2 * (Shape.C.x - Shape.A.x);
	P.y = Shape.A.y + r1 * (Shape.B.y - Shape.A.y) + r2 * (Shape.C.y - Shape.A.y);
	return (P);
}

void normalize_julia(void)
{
	julia_x = ((slider_x - 1530.0) / (1900.0 - 1530.0) * 4)-2;
	julia_y = ((slider_y - 630.0) / 370.0 * 4)-2;
}