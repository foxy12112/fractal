#include "fractal.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
	float x;
	float y;
} point;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} button;

//locations
//0 = main menu
//1 = sierpinski triangle

int location = 0;

static float g_zoom = 1.0f;
static int g_needs_clear = 0;

static const button g_triangle_button = {760.0f, 620.0f, 400.0f, 100.0f};
static const button g_exit_button = {760.0f, 460.0f, 400.0f, 100.0f};

point TOP = {WIDTH/2, HEIGHT*3/4};
point LEFT = {WIDTH/4, HEIGHT/4};
point RIGHT = {WIDTH*3/4, HEIGHT/4};

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

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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
	if (location != 0)
		return;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	x = (float)mouse_x;
	y = HEIGHT - (float)mouse_y;
	if (point_in_button(x, y, g_triangle_button))
	{
		location = 1;
		g_needs_clear = 1;
	}
	else if (point_in_button(x, y, g_exit_button))
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void draw_triangle()
{
	point a = zoom_point(TOP);
	point b = zoom_point(LEFT);
	point c = zoom_point(RIGHT);

	glBegin(GL_LINE_LOOP);
	glVertex3f(a.x, a.y, 0);
	glVertex3f(b.x, b.y, 0);
	glVertex3f(c.x, c.y, 0);
	glEnd();
}

void Main_menu()
{
	draw_button(g_triangle_button, 0.80f, 0.80f, 0.80f);
	draw_button(g_exit_button, 0.75f, 0.75f, 0.75f);
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
	if (location == 1)
		make_fractal_triangle();
}

point random_point_in_triangle(point A, point B, point C)
{
	float r1 = (float)rand() / (float)RAND_MAX;
	float r2 = (float)rand() / (float)RAND_MAX;
	if (r1 + r2 > 1)
	{
		r1 = 1.0f - r1;
		r2 = 1.0f - r2;
	}
	point P;
	P.x = A.x + r1 * (B.x - A.x) + r2 * (C.x - A.x);
	P.y = A.y + r1 * (B.y - A.y) + r2 * (C.y - A.y);
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
		current = random_point_in_triangle(TOP, LEFT, RIGHT);
		seeded = 1;
	}
	glPointSize(1);
	glBegin(GL_POINTS);
	i = 0;
	while (i < 20000)
	{
		int pick = rand() % 3;
		if (pick == 0)
			vertex = TOP;
		else if (pick == 1)
			vertex = LEFT;
		else
			vertex = RIGHT;
		current.x = (current.x + vertex.x) / 2;
		current.y = (current.y + vertex.y) / 2;
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