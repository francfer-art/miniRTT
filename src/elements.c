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
	fov = atof(data[3]);
	if (fov < 0 || fov > 180)
		return (free(camera), NULL);
	camera->fov = degree_2_rad(fov);
	camera->direction = ft_atov(data[2]);
	camera->origin = ft_atov(data[1]);
	if (out_range_vector(camera->direction))
		message_exit(ERROR_VECTOR);
	camera->direction = norm(camera->direction);
	return (camera);
}

t_light	*new_light(char **data)
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
		message_exit(ERROR_BRIGHTNESS);
	light->position = ft_atov(data[1]);
	return (light);
}

t_light	*new_ambient_light(char **data)
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
		message_exit(ERROR_BRIGHTNESS);
	return (light);
}
