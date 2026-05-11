#include "2dfractal.h"

void make_fractal_rectangle(void)
{
	static int seeded = 0;
	static point current;
	point vertex;
	point plotted;
	int i;

	if (!seeded)
	{
		srand((unsigned int)time(NULL));
		current = random_point_in_rectangle();
		seeded = 1;
	}
	glPointSize(1);
	glBegin(GL_POINTS);
	i = 0;
	while(i < max_iteration)
	{
		int pick = rand() % 8;
		switch (pick)
		{
		case 0:
			vertex = Shape.A;
			break;
		case 1:
			vertex = Shape.B;
			break;
		case 2:
			vertex = Shape.C;
			break;
		case 3:
			vertex = Shape.D;
			break;
		case 4:
			vertex.x = WIDTH/2;
			vertex.y = Shape.B.y;
			break;
		case 5:
			vertex.x = WIDTH/2;
			vertex.y = Shape.D.y;
			break;
		case 6:
			vertex.x = Shape.A.x;
			vertex.y = HEIGHT/2;
			break;
		case 7:
			vertex.x = Shape.B.x;
			vertex.y = HEIGHT/2;
			break;
		}
		current.x = current.x + (2.0f/3.0f) * (vertex.x - current.x);
		current.y = current.y + (2.0f/3.0f) * (vertex.y - current.y);
		plotted = zoom_point(current);
		glVertex3f(plotted.x, plotted.y, 0);
		i++;
	}
	glEnd();
}


void make_fractal_triangle(void)
{
	static int seeded = 0;
	static point current;
	point vertex;
	point plotted;
	int i;

	if (!seeded)
	{
		srand((unsigned int)time(NULL));
		current = random_point_in_triangle();
		seeded = 1;
	}
	glPointSize(1);
	glBegin(GL_POINTS);
	i = 0;
	while (i < max_iteration)
	{
		int pick = rand() % 3;
		if (pick == 0)
			vertex = Shape.A;
		else if (pick == 1)
			vertex = Shape.B;
		else
			vertex = Shape.C;
		current.x = (current.x + vertex.x) / 2;
		current.y = (current.y + vertex.y) / 2;
		plotted = zoom_point(current);
		glVertex3f(plotted.x, plotted.y, 0);
		i++;
	}
	glEnd();
}

void *mandelbrot_thread(void *arg)
{
	pixel_data *data = (pixel_data *)arg;
	pthread_mutex_lock(&start);
	pthread_mutex_unlock(&start);
	for (int y = data->y_start; y < data->y_end; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			point z = {0, 0};
			point c;
			int i = 0;
			bool escaped = false;
			c.x = (scale(x, -2, +2, WIDTH) / g_zoom) + shift_x;
			c.y = (scale(y, +2, -2, HEIGHT) / g_zoom) + shift_y;
			while(i < max_iteration)
			{
				z = sum_complex(square_complex(z), c);
				if ((z.x * z.x) + (z.y * z.y) > escape_value)
				{
					data->col[x][y] = colorize(scale(i, BLACK, WHITE, max_iteration));
					escaped = true;
					break ;
				}
				i++;
			}
			if (!escaped)
				data->col[x][y] = colorize(BLACK);
		}
	}
	return (NULL);
}