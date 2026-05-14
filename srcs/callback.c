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
	if (key == GLFW_KEY_KP_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (slider_x > WIDTH-390)
			slider_x--;
	if (key == GLFW_KEY_KP_6 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (slider_x < WIDTH-20)
			slider_x++;
	if (key == GLFW_KEY_KP_8 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (slider_y < HEIGHT - 85)
			slider_y++;
	if (key == GLFW_KEY_KP_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (slider_y > HEIGHT - 450)
			slider_y--;
	movement(key, action);
}
