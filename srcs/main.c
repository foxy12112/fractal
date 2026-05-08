#include "main.h"
#include "text.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
	double x;
	double y;
	double width;
	double height;
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

static double g_zoom = 1.0f;
static int g_needs_clear = 0;
static double shift_y = 0;
static double shift_x = 0;
static double escape_value = 4.0;
static int max_iteration = 200;
static bool need_redraw = false;
static bool g_drag_active = false;
static double g_drag_start_x = 0.0;
static double g_drag_start_y = 0.0;
static double deltatime = 0;
pthread_mutex_t start;

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
	out.x = (p.x + shift_x - WIDTH / 2.0f) * g_zoom + WIDTH / 2.0f;
	out.y = (p.y + shift_y - HEIGHT / 2.0f) * g_zoom + HEIGHT / 2.0f;
	return (out);
}

static void zoom_in_func()
{
	g_zoom *= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	need_redraw = true;
}

static void zoom_out_func()
{
	g_zoom /= 1.1;
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	need_redraw = true;
}

static int point_in_button(double x, double y, button rect)
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

static void draw_button(button rect, double red, double green, double blue)
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

int move_left = 0;
int move_right = 0;
int move_down = 0;
int move_up = 0;

static void movement(int key, int action)
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

void update(void)
{
	if (move_left)
		shift_x -= deltatime;
	else if (move_up)
		shift_y -= deltatime;
	else if (move_down)
		shift_y += deltatime;
	else if (move_right)
		shift_x += deltatime;
	g_needs_clear = 1;
	need_redraw = true;
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
	movement(key, action);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
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

void draw_outline(void)
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

void main_menu(void)
{
	for (int i = 0; i < 5; i++)
	{
		draw_button(g_menu_buttons[i].rect, 0.85f, 0.85f, 0.85f);
		write_button_text(g_menu_buttons[i].rect, g_menu_buttons[i].label);
	}
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

void render_loop(void)
{
	glPointSize(1);
	glLineWidth(2.5);
	update();
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
		main_menu();
	}
	else if (location == LOCATION_TRIANGLE && need_redraw)
	{
		draw_outline();
		make_fractal_triangle();
		need_redraw = false;
	}
	else if (location == LOCATION_RECTANGLE && need_redraw)
	{
		draw_outline();
		make_fractal_rectangle();
		need_redraw = false;
	}
	else if (location == LOCATION_MANDELBROT && need_redraw)
	{
		int thread_count = 16;
		pthread_t threads[thread_count];
		pixel_data pixels[thread_count];
		color **col = malloc(sizeof(color *) * WIDTH);
		for (int x = 0; x < WIDTH; x++)
			col[x] = malloc(sizeof(color) * HEIGHT);
		pthread_mutex_lock(&start);
		int rows_per_thread = HEIGHT/thread_count;

		for (int i = 0; i < thread_count; i++)
		{
			pixels[i].col = col;
			pixels[i].y_start = i * rows_per_thread;
			pixels[i].y_end = (i == thread_count - 1) ? HEIGHT : (i + 1) * rows_per_thread;
			pthread_create(&threads[i], NULL, mandelbrot_thread, &pixels[i]);
		}
		pthread_mutex_unlock(&start);
		for (int i = 0; i < thread_count; i++)
			pthread_join(threads[i], NULL);
		glBegin(GL_POINTS);
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				glColor3f(col[x][y].r, col[x][y].g, col[x][y].b);
				glVertex3f(x, y, 0);
			}
		}
		glEnd();
		for (int x = 0; x < WIDTH; x++)
			free(col[x]);
		free(col);
		need_redraw = false;
	}
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

double get_time()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + ts.tv_nsec / 1e9);
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
	main_menu();
	double last_time = get_time();
	double timer = 0.0;
	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		double current_time = get_time();
		deltatime = current_time - last_time;
		last_time = current_time;
		timer += deltatime;
		frames++;
		render_loop();
		glFlush();
		glfwPollEvents();
		if (timer >= 0.5)
		{
			double fps = frames / timer;
			printf("FPS: %.2f\n", fps);
			frames = 0;
			printf("zoom==%.2f\n", g_zoom);
			timer = 0.0;
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
	
}