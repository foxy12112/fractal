#include "gui.h"

void main_menu(void)
{
	for (int i = 0; i < 5; i++)
	{
		draw_button(g_menu_buttons[i].rect, 0.85f, 0.85f, 0.85f);
		write_button_text(g_menu_buttons[i].rect, g_menu_buttons[i].label);
	}
}

int point_in_button(double x, double y, button rect)
{
	if (x < rect.x)
		return (0);
	if (x > rect.x + rect.width)
		return (0);
	if (y < rect.y)
		return (0);
	if (y > rect.y + rect.height)
		return (0);
	return (1);
}

void draw_button(button rect, double red, double green, double blue)
{
	glColor3f(red, green, blue);
	glBegin(GL_QUADS);
	glVertex3f(rect.x, rect.y, 0.0f);
	glVertex3f(rect.x + rect.width, rect.y, 0.0f);
	glVertex3f(rect.x + rect.width, rect.y + rect.height, 0.0f);
	glVertex3f(rect.x, rect.y + rect.height, 0.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3.0f);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rect.x, rect.y, 0.0f);
	glVertex3f(rect.x + rect.width, rect.y, 0.0f);
	glVertex3f(rect.x + rect.width, rect.y + rect.height, 0.0f);
	glVertex3f(rect.x, rect.y + rect.height, 0.0f);
	glEnd();
}

void write_button_text(button rect, const char *label)
{
	double pixel_size;
	double text_width;
	point start;

	pixel_size = 4.0f;
	text_width = (double)strlen(label) * 6.0f * pixel_size;
	start.x = rect.x + (rect.width - text_width) * 0.5f;
	start.y = rect.y + (rect.height - 7.0f * pixel_size) * 0.5f;
	glColor3f(0.0f, 0.0f, 0.0f);
	write_text(label, pixel_size, start);
}

