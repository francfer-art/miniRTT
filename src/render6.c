#include <minirt.h>

float	calculate_v(int height, int j, int n)
{
	float	v;

	v = (float)j + ((float)n + 0.5) / (float)SQRT_SAMPLES;
	v = v / (float)height;
	return (v);
}

void	initialize_colors(t_color pixel_colors[SAMPLES_PER_PIXEL])
{
	int	k;

	k = 0;
	while (k < SAMPLES_PER_PIXEL)
	{
		pixel_colors[k] = 0;
		k++;
	}
}

void	show_menu(t_server *server)
{
	printf("-----------------------\n");
	if (server->world->checkerboard)
		printf("%sCheckerboard is on 🙿%s\n", GREEN, RESET);
	else
		printf("%sCheckerboard is off 🙿%s\n", RED, RESET);
	if (server->world->material)
		printf("%sMaterial is on 🔨%s\n", GREEN, RESET);
	else
		printf("%sMaterial is off 🔨%s\n", RED, RESET);
	if (server->world->phong)
		printf("%sPhong effect is on 🔦%s\n", GREEN, RESET);
	else
		printf("%sPhong effect is off 🔦%s\n", RED, RESET);
	if (server->world->bump)
		printf("%sBump texture is on 🧱%s\n", GREEN, RESET);
	else
		printf("%sBump texture is off 🧱%s\n", RED, RESET);
	if (server->world->texture)
		printf("%sTexture is on 🗺%s\n", GREEN, RESET);
	else
		printf("%sTexture is off 🗺%s\n", RED, RESET);
	printf("-----------------------\n");
}

void	render_selector(int t, t_thread_data *thread_data, t_server *server)
{
	int			rc;
	pthread_t	threads[NUM_THREADS];
	int			rows_per_thread;

	rows_per_thread = server->height / NUM_THREADS;
	if (t == NUM_THREADS - 1)
		thread_data[t].end_row = server->height;
	else
		thread_data[t].end_row = thread_data[t].start_row + rows_per_thread;
	if (server->world->material)
		rc = pthread_create(&threads[t], NULL, render_section_super,
				(void *)&thread_data[t]);
	else
		rc = pthread_create(&threads[t], NULL, render_section_super,
				(void *)&thread_data[t]);
}

void	check_pthread_create(int rc, t_server *server)
{
	if (rc)
		full_message_exit(ERROR_CREATE, NULL, server);
}
