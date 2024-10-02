#include <minirt.h>

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
