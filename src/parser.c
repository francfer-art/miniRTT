#include <minirt.h>
#include <stdio.h>
// Función para obtener la resolución de un mapa, los valores de la resolución
// estarán en la posición 1 y 2 del char **
// Primero se chequea la longitud del char **, si es distinto de 3 mostramos un 
// error y salimos. En otro caso alojamos la memoria para el array de enteros y
// rellenamos dicha memoria. Comprobamos que la resolución no es negativa
// La función devuelve un array de enteros con la resolución
int	*resolution(char **data, t_world *world)
{
	int	*res;

	if (double_pointer_len(data) != 3)
	{
		free_double(data);
		full_message_exit(ERROR_RES_LEN, world, NULL);
	}
	res = malloc(sizeof(*res) * 2);
	if (!res)
		return (NULL);
	res[0] = ft_atoi(data[1]);
	res[1] = ft_atoi(data[2]);
	if (res[0] <= 0 || res[1] <= 0)
	{
		free_double(data);
		full_message_exit(ERROR_RES_VALUE, world, NULL);
	}
	return (res);
}

// Función similar a scene_parser, con la única diferencia que esta agrupa todas
// las figuras de la escena. Pero en esencia es bastante parecida, identifica el 
// objeto a parsear, lo crea, lo rellena con la información del char ** y lo
// asigna a la estructura world
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
	else if (!ft_strcmp(line[0], "to"))
		ft_lstadd_back(&(world->figures), ft_lstnew(new_torus(line)));
	else if (!ft_strcmp(line[0], "co"))
	{
	    printf("pre new cone!\n");
		ft_lstadd_back(&(world->figures), ft_lstnew(new_cone(line)));
	}
	else
	{
		free_double(line);
		full_message_exit(ERROR_ID, world, NULL);
	}
}

// Función que se utiliza como selector de los objetos que entrarán en juego en
// la escena
// En resumen la función comprobará la primera posición del char ** para comprobar
// de que elemento se trata.
// Llamaremos a la función que crea el elemento y lo asignaremos a la estructura
// world
void	scene_parser(char **line, t_world *world)
{
	if (!ft_strcmp(line[0], "R"))
		world->resolution = resolution(line, world);
	else if (!ft_strcmp(line[0], "A"))
		world->ambient = new_ambient_light(line, world);
	else if (!ft_strcmp(line[0], "c"))
		ft_lstadd_back(&(world->cameras), ft_lstnew(new_camera(line)));
	else if (!ft_strcmp(line[0], "l"))
		ft_lstadd_back(&(world->lights), ft_lstnew(new_light(line,world)));
	else
		figures_parser(line, world);
}

// Función que parsea el archivo pasado por argumento
// Lo que vamos a hacer es leer línea a línea, mientras la línea exista y el primer
// caracter sea distinto de # realizaremos lo siguiente:
// Splitearemos la línea por espacios, parsearemos ese split y lo liberaremos,
// liberamos la línea obtenida con GNL y volvemos a llamar a GNL para la siguiente
// vuelta
// Si no hay resolución o no hay luz de ambiente, mostramos el error adecuado y
// salimos. Si no hay errores llegamos al return y devolvemos 0, indicando que 
// el parseo ha sido exitoso.
int	parser_file(int fd, t_world *world)
{
	char	*line;
	char	**split;

	line = NULL;
	split = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (*line && *line != '#' && *line != '\n')
		{
			split = ft_split_space(line);
			scene_parser(split, world);
			free_double(split);
		}
		free(line);
		line = get_next_line(fd);
	}
	if (!world->resolution)
		full_message_exit(ERROR_RES, world, NULL);
	else if (!world->ambient)
		full_message_exit(ERROR_AMB, world, NULL);
	return (0);
}

// int parser_file(int fd, t_world *world)
// {
// 	char *line;

// 	line = NULL;

// 	while((line = get_next_line(fd)))
// 	{
// 		printf("%s\n", line);
// 		free(line);
// 	}

// 	return 0;
// }


// int	parser_file(int fd, t_world *world)
// {
// 	char	*line;
// 	char	**split;

// 	line = NULL;
// 	split = NULL;
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		if (*line && *line != '#' && *line != '\n')
// 		{
// 			split = ft_split_space(line);
// 			free(line);
// 			scene_parser(split, world);
// 			free_double(split);
// 		}
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	if (!world->resolution)
// 		full_message_exit(ERROR_RES, world, NULL);
// 	else if (!world->ambient)
// 		full_message_exit(ERROR_AMB, world, NULL);
// 	return (0);
// }
