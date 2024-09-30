#include <minirt.h>

t_camera	*new_camera(char **data)
{
	t_camera	*camera;
	float		fov;

	if (double_pointer_len(data) != 4)
		return (NULL);
	camera = malloc(sizeof(*camera));
	if (!camera)
		return (NULL);
	fov = ft_atof(data[3]);
	if (fov < 0 || fov > 180)
		return (free(camera), NULL);
	camera->fov = degree_2_rad(fov);
	camera->direction = norm(ft_atov(data[2]));
	camera->origin = ft_atov(data[1]);
	return (camera);
}

t_light	*new_light(char **data, t_world *world)
{
	t_light	*light;

	if (double_pointer_len(data) != 4)
		return (NULL);
	light = malloc(sizeof(*light));
	if (!light)
		return (NULL);
	light->color = ft_atoc(data[3]);
	light->brightness = ft_atof(data[2]);
	if (light->brightness < 0 || light->brightness > 1)
		full_message_exit(ERROR_BRIGHTNESS, world, NULL);
	light->position = ft_atov(data[1]);
	return (light);
}

t_light	*new_ambient_light(char **data, t_world *world)
{
	t_light	*light;

	if (double_pointer_len(data) != 3)
		return (NULL);
	light = malloc(sizeof(*light));
	if (!light)
		return (NULL);
	light->color = ft_atoc(data[2]);
	light->brightness = ft_atof(data[1]);
	if (light->brightness < 0 || light->brightness > 1)
		full_message_exit(ERROR_BRIGHTNESS, world, NULL);
	return (light);
}

void	compute_sample_colors(t_thread_data *data, int i, int j,
		t_color pixel_colors[SAMPLES_PER_PIXEL])
{
	t_ray	ray;
	t_color	sample_color;
	int		vars[3];
	float	u;
	float	v;

	vars[0] = 0;
	vars[2] = 0;
	while (vars[0] < SQRT_SAMPLES)
	{
		vars[1] = 0;
		while (vars[1] < SQRT_SAMPLES)
		{
			u = ((float)i + (vars[0] + 0.5) / SQRT_SAMPLES)
				/ data->server->width;
			v = ((float)j + (vars[1] + 0.5) / SQRT_SAMPLES)
				/ data->server->height;
			ray = generate_ray(data->server->world->cameras->content, u, v);
			sample_color = raytracer(&ray, data->server->world, MAX_DEPTH);
			pixel_colors[vars[2]] = sample_color;
			vars[2]++;
			vars[1]++;
		}
		vars[0]++;
	}
}
