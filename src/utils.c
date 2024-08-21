#include <minirt.h>

// Función para calcular la longitud de un char **
int	double_pointer_len(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	return (i);
}

// Función para saber si un vector está fuera de rango
// Devuelve 1 si el vector está fuera de rango y 0 si está dentro de rango
// Considerando vector fuera de rango si alguna de sus componentes es menor que -1 
// y mayor que 1
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

// Función para transformar los grados en radianes
float	degree_2_rad(float degree)
{
	return ((M_PI / 180) * degree);
}

// Función para transformar un string en un float
// "23.4" --> 23.4
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
		message_exit(ERROR_DIR);
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

// Función para transoformar un string en un t_vector
// "0, 1, 1" --> [0, 1, 1]
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
