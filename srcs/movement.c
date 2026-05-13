#include "movement.h"

void update(void)
{
	if (move_left)
		shift_x -= deltatime / g_zoom;
	if (move_up)
		shift_y -= deltatime / g_zoom;
	if (move_down)
		shift_y += deltatime / g_zoom;
	if (move_right)
		shift_x += deltatime / g_zoom;
	g_needs_clear = 1;
	need_redraw = true;
}

void movement(int key, int action)
{
	if (key == GLFW_KEY_LEFT)
		if (action == GLFW_PRESS)
			move_left = 1;
		else if (action == GLFW_RELEASE)
			move_left = 0;
	if (key == GLFW_KEY_RIGHT)
		if (action == GLFW_PRESS)
			move_right = 1;
		else if (action == GLFW_RELEASE)
			move_right = 0;
	if (key == GLFW_KEY_UP)
		if (action == GLFW_PRESS)
			move_up = 1;
		else if (action == GLFW_RELEASE)
			move_up = 0;
	if (key == GLFW_KEY_DOWN)
		if (action == GLFW_PRESS)
			move_down = 1;
		else if (action == GLFW_RELEASE)
			move_down = 0;
}
