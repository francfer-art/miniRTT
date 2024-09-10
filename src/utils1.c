#include <minirt.h>

// Función para liberar un char **
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

// Función para saber si un número es inválido, devuelve 1 si es inválido y 0 si es 
// válido. En este caso estoy tratando números decimales, es decir, 23.4 es válido
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
	int i = 0;
	while(data[i])
		ft_printf("%s\n", data[i++]);
}

// Función para transformar un string en un t_color
// "0,0,255" --> 255
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
		// printf("here!\n");
		free_double(split);
		// msg_exit(ERROR_COLOR);
		return -1;
	}
	else
	{
		while (i < 3)
		{
			if (invalid_number(split[i]))
				msg_exit(ERROR_DIR);
			prim = ft_atoi(split[i]);
			if (prim < 0 || prim > 0xFF)
				msg_exit(ERROR_COL_VAL);
			color = (color << 8) | prim;
			i++;
		}
	}
	free_double(split);
	return (color);
}

// Función para normalizar un vector, esto significa que el vector tiene de módulo 1
// Básicamente divide cada componente, por su módulo
t_vector	norm(t_vector v)
{
	return (scale(v, 1 / length(v)));
}

//Función que "escala" un vector, multiplica todas sus componentes por un escalar
t_vector	scale(t_vector v, double c)
{
	return (vector(v.x * c, v.y * c, v.z * c));
}
