#include "callback.h"

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	double mouse_x;
	double mouse_y;
	double x;
	double y;

	(void)mods;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	x = (double)mouse_x;
	y = HEIGHT - (double)mouse_y;
	if (location == LOCATION_MENU)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!point_in_button(x, y, g_menu_buttons[i].rect))
				continue;
			location = g_menu_buttons[i].location;
			g_needs_clear = 1;
			need_redraw = true;
			if (location == LOCATION_TRIANGLE)
			{
				Shape.A.x = WIDTH / 2.0f;
				Shape.A.y = HEIGHT * 3.0f / 4.0f;
				Shape.B.x = WIDTH / 4.0f;
				Shape.B.y = HEIGHT / 4.0f;
				Shape.C.x = WIDTH * 3.0f / 4.0f;
				Shape.C.y = HEIGHT / 4.0f;
				Shape.points = 3;
			}
			if (location == LOCATION_RECTANGLE)
			{
				Shape.A.x = WIDTH/4;
				Shape.A.y = HEIGHT/4;
				Shape.B.x = WIDTH*3/4;
				Shape.B.y = HEIGHT/4;
				Shape.C.x = WIDTH*3/4;
				Shape.C.y = HEIGHT*3/4;
				Shape.D.x = WIDTH/4;
				Shape.D.y = HEIGHT*3/4;
				Shape.points = 4;
			}
			if (location == LOCATION_EXIT)
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}
	}
	else
	{
		if (action == GLFW_PRESS)
		{
			g_drag_active = true;
			g_drag_start_x = x;
			g_drag_start_y = y;
		}
		else if (action == GLFW_RELEASE && g_drag_active)
		{
			
			double cx = (g_drag_start_x + x) * 0.5;
			double cy = (g_drag_start_y + y) * 0.5;

			shift_x = (scale((int)cx, -2, +2, WIDTH) * g_zoom) + shift_x;
			shift_y = (scale((int)cy, +2, -2, HEIGHT) * g_zoom) + shift_y;

			g_drag_active = false;
			zoom_in_func();
		}

	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		zoom_in_func();
	else if (yoffset < 0)
		zoom_out_func();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	(void)window;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		location = LOCATION_MENU;
		g_needs_clear = 1;
	}
	if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
		max_iteration++;
	if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		max_iteration--;
	movement(key, action);
}
