#include <minirt.h>

void	free_double(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i++]);
	}
	free(s);
}

int	invalid_number(char *str)
{
	int	inv;
	int	i;

	i = -1;
	inv = 0;
	str = ft_strtrim(str, "\n");
	while (str[++i])
	{
		if (i == 0)
			inv |= (!ft_isdigit(str[i]) & (str[i] != '+') & (str[i] != '-'));
		else
			inv |= (!ft_isdigit(str[i]) & (str[i] != '.'));
	}
	free(str);
	return (inv);
}

void	show_split_content(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		ft_printf("%s\n", data[i++]);
}

t_color	ft_atoc(char *str)
{
	t_color	color;
	char	**split;
	int		i;
	int		prim;

	i = 0;
	color = 0x0;
	split = ft_split(str, ',');
	if (double_pointer_len(split) != 3)
	{	
		free_double(split);
		return -1;
	}
	else
	{
		while (i < 3)
		{
			if (invalid_number(split[i]))
			{
				free_double(split);
				return -1;
			}
			prim = ft_atoi(split[i]);
			if (prim < 0 || prim > 0xFF)
			{
				free_double(split);
				return -1;
			}
			color = (color << 8) | prim;
			i++;
		}
	}
	free_double(split);
	return (color);
}

t_vector	norm(t_vector v)
{
	return (scale(v, 1 / length(v)));
}
