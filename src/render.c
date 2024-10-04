#include <minirt.h>

int	intersec(t_ray *ray, t_list *figures)
{
	int		hit;
	t_type	type;

	hit = 0;
	ray->record.t = INFINITY;
	while (figures)
	{
		type = *((t_type *)figures->content);
		if (type == CYLINDER)
			hit |= hit_cylinder(ray, (t_cylinder *)figures->content);
		else if (type == PLANE)
			hit |= hit_plane(ray, (t_plane *)figures->content);
		else if (type == SPHERE)
			hit |= hit_sphere(ray, (t_sphere *)figures->content);
		else if (type == SQUARE)
			hit |= hit_square(ray, (t_square *)figures->content);
		else if (type == TRIANGLE)
			hit |= hit_triangle(ray, (t_triangle *)figures->content);
		else if (type == CONE)
		{
			hit |= hit_cone(ray, (t_cone *)figures->content);
		}
		figures = figures->next;
	}
	return (hit);
}

void	*render_section(void *threadarg)
{
	t_thread_data	*data;
	int				i;
	int				j;
	t_color			pixel_color;
	t_ray			ray;

	ft_memset(&ray, 0, sizeof(t_ray));
	data = (t_thread_data *)threadarg;
	j = data->start_row;
	while (j < data->end_row)
	{
		i = 0;
		while (i < data->server->width)
		{
			ray = generate_ray(data->server->world->cameras->content, (float)i
					/ data->server->width, (float)j / data->server->height);
			pixel_color = raytracer(&ray, data->server->world, 5);
			my_put_pixel(data->server, i, data->server->height - 1 - j,
				pixel_color);
			i++;
		}
		j++;
	}
	pthread_exit(NULL);
}

t_color	average_color(t_color *colors, int num_colors)
{
	int	r_sum;
	int	g_sum;
	int	b_sum;
	int	i;

	r_sum = 0;
	g_sum = 0;
	b_sum = 0;
	i = -1;
	while (++i < num_colors)
	{
		r_sum += (colors[i] >> 16) & 0xFF;
		g_sum += (colors[i] >> 8) & 0xFF;
		b_sum += colors[i] & 0xFF;
	}
	return (((r_sum / num_colors) << 16) | ((g_sum / num_colors)) << 8 | (b_sum
			/ num_colors));
}

void	render(t_server *server)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				rows_per_thread;
	int				rc;
	int				t;

	check_server(server, &t, &rows_per_thread);
	while (++t < NUM_THREADS)
	{
		thread_data[t].server = server;
		thread_data[t].start_row = t * rows_per_thread;
		if (t == NUM_THREADS - 1)
			thread_data[t].end_row = server->height;
		else
			thread_data[t].end_row = thread_data[t].start_row + rows_per_thread;
		if (server->world->material)
			rc = pthread_create(&threads[t], NULL, render_section_super,
					(void *)&thread_data[t]);
		else
			rc = pthread_create(&threads[t], NULL, render_section,
					(void *)&thread_data[t]);
		check_pthread_create(rc, server);
	}
	join_menu_image(threads, server);
}

void	render_low(t_server *server)
{
	int		i;
	int		j;
	int		scale_factor;
	t_ray	ray;
	t_color	pixel_color;

	scale_factor = adjust_scale_factor(server);
	if (!server->world->cameras || scale_factor <= 0)
		return ;
	j = -1;
	while (++j < server->height / scale_factor)
	{
		i = -1;
		while (++i < server->width / scale_factor)
		{
			ray = generate_ray(server->world->cameras->content, (float)i
					/ (server->width / scale_factor), (float)j / (server->height
						/ scale_factor));
			pixel_color = raytracer(&ray, server->world, MAX_DEPTH);
			fill_pixels(server, i, j, pixel_color);
		}
	}
	mlx_put_image_to_window(server->mlx, server->window, server->image->image,
		0, 0);
}
