#include "fractal.h"
#include <stdlib.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
	float x;
	float y;
} point;

static float g_zoom = 1.0f;

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

void render_loop()
{
	glPointSize(1);
	glLineWidth(2.5);
	glColor3f(0, 0, 0);
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

	glViewport(0,0,WIDTH,HEIGHT);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0,WIDTH,0,HEIGHT,0,1);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//draw_triangle();
	glFlush();
	while (!glfwWindowShouldClose(window))
	{
		render_loop();
		//glfwSwapBuffers(window);
		glFlush();
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
	
}