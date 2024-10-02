#include <minirt.h>

int	double_pointer_len(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	return (i);
}

int	out_range_vector(t_vector v)
{
	int	out;

	out = 0;
	if (v.x < -1 || v.x > 1)
		out = 1;
	if (v.y < -1 || v.y > 1)
		out = 1;
	if (v.z < -1 || v.z > 1)
		out = 1;
	return (out);
}

float	degree_2_rad(float degree)
{
	return ((M_PI / 180) * degree);
}

float	ft_atof(char *str)
{
	float	num;
	float	d;
	int		i;
	int		sign;

	num = 0;
	d = 0.0;
	i = -1;
	sign = 1;
	if (invalid_number(str))
		return 1;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (ft_isdigit(*str))
		num = (num * 10) + (*str++ - '0');
	if (*str == '.' && *str++)
	{
		while (ft_isdigit(*str))
			d += (pow(10, i--) * (*str++ - '0'));
	}
	return ((num + d) * sign);
}

t_vector	ft_atov(char *str)
{
	t_vector	v;
	char		**split;

	split = ft_split(str, ',');
	if (double_pointer_len(split) != 3)
		v = vector(0, 0, 0);
	else
	{
		v.x = ft_atof(split[0]);
		v.y = ft_atof(split[1]);
		v.z = ft_atof(split[2]);
	}
	free_double(split);
	return (v);
}
