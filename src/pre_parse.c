#include <minirt.h>

int	get_number_lines(char *file)
{
	int		fd;
	char	*line;
	int		count_line;

	count_line = 0;
	fd = open_scene_file(file);
	if (fd < 0)
		msg_exit(ERROR_OPEN);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '#' && *line != '\n')
		{
			count_line++;
		}
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	if (close(fd) < 0)
		msg_exit(ERROR_OPEN);
	return (count_line);
}



char	**load_map(int n_lines, char *file)
{
	char	**full_map;
	int		fd;
	char	*line;
	int		i;

	i = 0;
	full_map = (char **)malloc(sizeof(char *) * (n_lines + 1));
	if (!full_map)
		return (NULL);
	fd = open_scene_file(file);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '#' && *line != '\n')
		{
			full_map[i] = ft_strdup(line);
			i++;
		}
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	full_map[i] = NULL;
	return (full_map);
}

int	ambient_checker(char **data)
{
	int	res;

	res = 0;
	if (!ft_strcmp(data[0], "R") && check_r(data) != 0)
		res = check_r(data);
	else if (!ft_strcmp(data[0], "A") && check_a(data) != 0)
		res = check_a(data);
	else if (!ft_strcmp(data[0], "c") && check_c(data) != 0)
		res = check_c(data);
	else if (!ft_strcmp(data[0], "l") && check_l(data) != 0)
		res = check_l(data);
	return (res);
}

int	check_errors(char **full_map)
{
	int		i;
	char	**data;
	int		res;

	i = -1;
	data = NULL;
	res = 0;
	while (full_map[++i] && res == 0)
	{
		data = ft_split_space(full_map[i]);
		if (ambient_checker(data) != 0)
			res = ambient_checker(data);
		else if (!ft_strcmp(data[0], "pl") && check_pl(data) != 0)
			res = check_pl(data);
		else if (!ft_strcmp(data[0], "sp") && check_sp(data) != 0)
			res = check_pl(data);
		else if (!ft_strcmp(data[0], "sq") && check_sq(data) != 0)
			res = check_sq(data);
		else if (!ft_strcmp(data[0], "cy") && check_cy(data) != 0)
			res = check_cy(data);
		free_double(data);
	}
	return (res);
}

int	pre_parse(char *file)
{
	int		count_line;
	char	**full_map;
	int		res;

	// char	*line;
	// int		i;
	// char	**data;
	count_line = 0;
	// line = NULL;
	// full_map = NULL;
	count_line = get_number_lines(file);
	full_map = load_map(count_line, file);
	res = check_errors(full_map);
	free_double(full_map);
	return (res);
}
