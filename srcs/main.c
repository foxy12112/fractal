#include "fractal.h"

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
		make_fractal_triangle();
		need_redraw = false;
	}
	else if (location == LOCATION_RECTANGLE && need_redraw)
	{
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
	else if (location == LOCATION_JULIA && need_redraw)
	{
		int thread_count = 16;
		pthread_t threads[thread_count];
		pixel_data pixels[thread_count];
		color **col = malloc(sizeof(color *) *WIDTH);
		for (int x = 0; x < WIDTH; x++)
			col[x] = malloc(sizeof(color) * HEIGHT);
		pthread_mutex_lock(&start);
		int rows_per_thread = HEIGHT/thread_count;
		for (int i = 0; i < thread_count; i++)
		{
			pixels[i].col = col;
			pixels[i].y_start = i * rows_per_thread;
			pixels[i].y_end = (i == thread_count - 1) ? HEIGHT : (i + 1) * rows_per_thread;
			pthread_create(&threads[i], NULL, simple_julia_thread, &pixels[i]);
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
		draw_button(slider, 211, 211, 211);
		draw_slider(slider, 211, 211, 211);
	}
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
	glfwSetScrollCallback(window, scroll_callback);

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
			printf("max iterations==%d\n", max_iteration);
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
	
}