#include "fractal.h"
#include "text.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
	float x;
	float y;
	float width;
	float height;
} button;

typedef struct {
	point A;
	point B;
	point C;
	point D;
	int points;
} shape;

typedef struct {
	button rect;
	int location;
	const char *label;
} menu_button;

//locations
//0 = main menu
//1 = sierpinski triangle

#define LOCATION_MENU 0
#define LOCATION_TRIANGLE 1
#define LOCATION_RECTANGLE 2
#define LOCATION_MANDELBROT 3
#define LOCATION_JULIA 4
#define LOCATION_EXIT 5

int location = LOCATION_MENU;

static float g_zoom = 1.0f;
static int g_needs_clear = 0;

static const menu_button g_menu_buttons[5] = {
	{{760.0f, 760.0f, 400.0f, 80.0f}, LOCATION_TRIANGLE, "TRIANGLE"},
	{{760.0f, 660.0f, 400.0f, 80.0f}, LOCATION_RECTANGLE, "RECTANGLE"},
	{{760.0f, 560.0f, 400.0f, 80.0f}, LOCATION_MANDELBROT, "MANDELBROT"},
	{{760.0f, 460.0f, 400.0f, 80.0f}, LOCATION_JULIA, "JULIA"},
	{{760.0f, 360.0f, 400.0f, 80.0f}, LOCATION_EXIT, "EXIT"}
};

shape Shape;


point zoom_point(point p)
{
	point out;
	out.x = (p.x - WIDTH / 2.0f) * g_zoom + WIDTH / 2.0f;
	out.y = (p.y - HEIGHT / 2.0f) * g_zoom + HEIGHT / 2.0f;
	return (out);
}

static void zoom_in_func()
{
	g_zoom *= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void zoom_out_func()
{
	g_zoom /= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static int point_in_button(float x, float y, button rect)
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

static void draw_button(button rect, float red, float green, float blue)
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

static void write_button_text(button rect, const char *label)
{
	float pixel_size;
	float text_width;
	point start;

	pixel_size = 4.0f;
	text_width = (float)strlen(label) * 6.0f * pixel_size;
	start.x = rect.x + (rect.width - text_width) * 0.5f;
	start.y = rect.y + (rect.height - 7.0f * pixel_size) * 0.5f;
	glColor3f(0.0f, 0.0f, 0.0f);
	write_text(label, pixel_size, start);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	(void)window;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		location = LOCATION_MENU;
		g_needs_clear = 1;
	}
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
		zoom_in_func();
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
		zoom_out_func();
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	double mouse_x;
	double mouse_y;
	float x;
	float y;

	(void)mods;
	if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
		return;
	if (location != LOCATION_MENU)
		return;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	x = (float)mouse_x;
	y = HEIGHT - (float)mouse_y;
	for (int i = 0; i < 5; i++)
	{
		if (!point_in_button(x, y, g_menu_buttons[i].rect))
			continue;
		location = g_menu_buttons[i].location;
		g_needs_clear = 1;
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

void draw_outline()
{
	point a = zoom_point(Shape.A);
	point b = zoom_point(Shape.B);
	point c = zoom_point(Shape.C);
	glBegin(GL_LINE_LOOP);
	glVertex3f(a.x, a.y, 0);
	glVertex3f(b.x, b.y, 0);
	glVertex3f(c.x, c.y, 0);
	if (Shape.points == 4)
	{
		point d = zoom_point(Shape.D);
		glVertex3f(d.x, d.y, 0);
	}
	glEnd();
}

void Main_menu()
{
	for (int i = 0; i < 5; i++)
	{
		draw_button(g_menu_buttons[i].rect, 0.85f, 0.85f, 0.85f);
		write_button_text(g_menu_buttons[i].rect, g_menu_buttons[i].label);
	}
}

void draw_mandelbrot()
{
	point z;
	point c;
	int i;
	i = 0;
	z.x = 0;
	z.y = 0;
	
}

void render_loop()
{
	glPointSize(1);
	glLineWidth(2.5);
	if (g_needs_clear)
	{
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		g_needs_clear = 0;
	}
	if (location == LOCATION_MENU)
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Main_menu();
	}
	else if (location == LOCATION_TRIANGLE)
	{
		draw_outline();
		make_fractal_triangle();
	}
	else if (location == LOCATION_RECTANGLE)
	{
		draw_outline();
		make_fractal_rectangle();
	}
	else if (location == LOCATION_MANDELBROT)
	{
		draw_mandelbrot();
	}
}

point random_point_in_triangle()
{
	float r1 = (float)rand() / (float)RAND_MAX;
	float r2 = (float)rand() / (float)RAND_MAX;
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

void make_fractal_triangle()
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
	while (i < 20000)
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

point random_point_in_rectangle()
{
	point p;
	float min_x = Shape.A.x;
	float max_x = Shape.B.x;
	float min_y = Shape.A.y;
	float max_y = Shape.D.y;

	printf("%f\n", max_x);
	p.x = min_x + (float)(rand() % 960);
	p.y = min_y + (float)(rand() % 540);
	return (p);
}

void make_fractal_rectangle()
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
	while(i < 20000)
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

int main(void)
{
	GLFWwindow *window;

	if (!glfwInit())
		return (1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "fractal", NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return (1);
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return (1);
	}
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glViewport(0,0,WIDTH,HEIGHT);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0,WIDTH,0,HEIGHT,0,1);
	Main_menu();
	while (!glfwWindowShouldClose(window))
	{
		render_loop();
		glFlush();
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
	
}