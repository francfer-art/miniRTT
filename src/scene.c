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

	world = malloc(sizeof(*world));
	if (!world)
		return (NULL);
	world->resolution = NULL;
	world->figures = NULL;
	world->lights = NULL;
	world->cameras = NULL;
	world->ambient = NULL;
	world->checkerboard = 0;
	world->material = 0;
	world->texture = 0;
	world->phong = 0;
	world->bump = 0;
	set_texture(world);
	return (world);
}

t_world	*scene_init(char *file)
{
	int		fd;
	int		err;
	t_world	*world;

	fd = open_scene_file(file);
	if (fd < 0)
		msg_exit(ERROR_OPEN);
	world = new_world();
	err = parser_file(fd, world);
	if (close(fd) < 0)
		full_message_exit(ERROR_CLOSE, world, NULL);
	if (err)
		full_message_exit(ERROR_PARSE, world, NULL);
	return (world);
}

void	init_texture(void *mlx_ptr, t_texture *texture, char *file_path)
{
	texture->img_ptr = mlx_xpm_file_to_image(mlx_ptr, file_path,
			&texture->width, &texture->height);
	if (!texture->img_ptr)
	{
		printf("Error: No se pudo cargar la textura desde %s\n", file_path);
		return ;
	}
	texture->img_data = mlx_get_data_addr(texture->img_ptr, &texture->bpp,
			&texture->size_line, &texture->endian);
}
