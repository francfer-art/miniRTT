#include <minirt.h>

int	*resolution(char **data)
{
	int	*res;

	if (double_pointer_len(data) != 3)
		message_exit(ERROR_RES_LEN);
	res = malloc(sizeof(*res) * 2);
	if (!res)
		return (NULL);
	res[0] = ft_atoi(data[1]);
	res[1] = ft_atoi(data[2]);
	if (res[0] <= 0 || res[1] <= 0)
		message_exit(ERROR_RES_VALUE);
	return (res);
}

void	figures_parser(char **line, t_world *world)
{
	if (!ft_strcmp(line[0], "pl"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_plane(line)));
	else if (!ft_strcmp(line[0], "sp"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_sphere(line)));
	else if (!ft_strcmp(line[0], "sq"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_square(line)));
	else if (!ft_strcmp(line[0], "cy"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_cylinder(line)));
	else if (!ft_strcmp(line[0], "tr"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_triangle(line)));
	else
		message_exit(ERROR_ID);
}

void	scene_parser(char **line, t_world *world)
{
	if (!ft_strcmp(line[0], "R"))
		world->resolution = resolution(line);
	else if (!ft_strcmp(line[0], "A"))
		world->ambient = new_ambient_light(line);
	else if (!ft_strcmp(line[0], "c"))
		ft_lstadd_back(&(world->cameras), ft_lstnew(new_camera(line)));
	else if (!ft_strcmp(line[0], "l"))
		ft_lstadd_back(&(world->lights), ft_lstnew(new_light(line)));
	else
		figures_parser(line, world);
}

int	parser_file(int fd, t_world *world)
{
	char	*line;
	char	**split;

	line = NULL;
	split = NULL;

	line = get_next_line(fd);
	while (line)
	{
		if (*line && *line != '#')
		{
			split = ft_split(line, ' ');
			scene_parser(split, world);
			free_double(split);
		}
		free(line);
		line = get_next_line(fd);
	}
	if (!world->resolution)
		message_exit(ERROR_RES);
	else if (!world->ambient)
		message_exit(ERROR_AMB);
	return (0);
}
