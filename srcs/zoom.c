#include "zoom.h"

point zoom_point(point p)
{
	point out;
	out.x = (p.x + shift_x - WIDTH / 2.0f) * g_zoom + WIDTH / 2.0f;
	out.y = (p.y + shift_y - HEIGHT / 2.0f) * g_zoom + HEIGHT / 2.0f;
	return (out);
}

void zoom_in_func()
{
	g_zoom *= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	need_redraw = true;
}

void zoom_out_func()
{
	g_zoom /= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	need_redraw = true;
}
