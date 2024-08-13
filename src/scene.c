#include <minirt.h>

int	open_scene_file(char *file)
{
	int		fd;
	char	*extension;

	fd = -1;
	extension = ft_strchr(file, '.');
	if (extension && !ft_strcmp(EXTENSION, extension))
	{
		fd = open(file, O_RDONLY);
		return (fd);
	}
	return (fd);
}

t_world	*new_world(void)
{
	t_world	*world;

	world = malloc(sizeof(t_world *));
	if (!world)
		return (NULL);
	world->resolution = NULL;
	world->figures = NULL;
	world->lights = NULL;
	world->cameras = NULL;
	world->ambient = NULL;
	return (world);
}

t_world	*scene_init(char *file)
{
	int		fd;
	int		err;
	t_world	*world;

	fd = open_scene_file(file);
	if (fd < 0)
		message_exit(ERROR_OPEN);
	world = new_world();
	err = parser_file(fd, world);
	if (close(fd) < 0)
		message_exit (ERROR_CLOSE);
	if (err)
		free_world(world);
	return (world);
}
