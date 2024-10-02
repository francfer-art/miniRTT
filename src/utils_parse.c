#include <minirt.h>

int	check_r(char **data)
{
	if (double_pointer_len(data) != 3)
		return (ERROR_RES_LEN);
	else if (ft_atoi(data[1]) <= 0 || ft_atoi(data[2]) <= 0)
		return (ERROR_RES_VALUE);
	return (0);
}

int	check_a(char **data)
{
	if (double_pointer_len(data) != 3)
		return (ERROR_RES_LEN);
	else if (ft_atof(data[1]) > 1 || ft_atoc(data[2]) < 0)
		return (ERROR_BRIGHTNESS);
	return (0);
}

int	check_c(char **data)
{
	if (double_pointer_len(data) != 4)
		return (ERROR_CAMERA);
	else if (ft_atof(data[3]) < 0 || ft_atof(data[3]) > 180)
		return (ERROR_CAMERA);
	return (0);
}

int	check_l(char **data)
{
	if (double_pointer_len(data) < 4)
		return (ERROR_PARSE);
	else if (ft_atoc(data[3]) == -1)
		return (ERROR_COLOR);
	return (0);
}

int	check_pl(char **data)
{
	if (double_pointer_len(data) != 4)
		return (ERROR_FORMAT);
	else if (ft_atoc(data[3]) == -1)
		return (ERROR_COLOR);
	return (0);
}

int	check_sp(char **data)
{
	if (double_pointer_len(data) < 4)
		return (ERROR_FORMAT);
	else if (ft_atoc(data[3]) == -1)
		return (ERROR_COLOR);
	else if (double_pointer_len(data) == 5 && ft_strcmp(data[4], "GLASS"))
		return (ERROR_FORMAT);
	return (0);
}

int	check_sq(char **data)
{
	if (double_pointer_len(data) < 5)
		return (ERROR_FORMAT);
	else if (ft_atoc(data[4]) == -1)
		return (ERROR_COLOR);
	else if (double_pointer_len(data) == 6 && ft_strcmp(data[5], "GLASS"))
		return (ERROR_FORMAT);
	return (0);
}

int	check_cy(char **data)
{
	if (double_pointer_len(data) < 6)
		return (ERROR_FORMAT);
	else if (ft_atoc(data[5]) == -1 || ft_atoc(data[5]) == -1)
		return (ERROR_COLOR);
	return (0);
}
