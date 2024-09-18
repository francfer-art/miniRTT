#include <minirt.h>

// Función principal del proyecto
// Lo primero que hacemos es verificar que el número de argumentos es válido,
// es decir, si el número de argumentos es menor que 2 o mayor que 3 paramos la
// es decir, si el número de argumentos es menor que 2 o mayor que 3 paramos la
// ejecucuión aquí.
// Inicializamos el mundo --> Esta función leerá el mapa y parseará toda la
// información e irá creando las figuras y guardandolas en la estructura world
// Creamos el server --> Esta función creará los punteros necesarios para
// que la comunicación con la minilibX sea correcta
// Si falla la creación del server --> Mensaje de error
// Renderizamos la escena con toda la información
// Llamamos la función que manejará los eventos --> El programa acabará de forma
// limpia cuando se pulse la tecla ESC o cuando cerremos la ventana

void	msg_exit(ErrorType error)
{
	char	*error_messages[ERROR_COUNT];

	error_messages[ERROR_JEFE] = strdup("🐷 Nos vemos jefe 🐷");
	error_messages[ERROR_MALLOC] = strdup("ERROR : Malloc problem 🔴");
	error_messages[ERROR_PARSE] = strdup("ERROR : While parsing 🔴");
	error_messages[ERROR_CREATE] = strdup("ERROR; return (code from pthread_create() 🔴");
	error_messages[ERROR_JOIN] = strdup("ERROR : return (code from pthread_join() 🔴");
	error_messages[ERROR_COLOR] = strdup("ERROR : Invalid color definition 🔴");
	error_messages[ERROR_SERVER] = strdup("ERROR : Can't create server 🔴");
	error_messages[ERROR_COL_VAL] = strdup("ERROR : Color value out range 🔴");
	error_messages[ERROR_VECTOR] = strdup("ERROR : Vector out range 🔴");
	error_messages[ERROR_BRIGHTNESS] = strdup("ERROR : Brightness out of range 🔴");
	error_messages[ERROR_RATIO] = strdup("ERROR : Ambient light ratio out of range 🔴");
	error_messages[ERROR_DIR] = strdup("ERROR : Invalid number detected 🔴");
	error_messages[ERROR_ARG] = strdup("ERROR : Incorrect number of arguments 🔴");
	error_messages[ERROR_ID] = strdup("ERROR : Invalid object's identifier 🔴");
	error_messages[ERROR_RES] = strdup("ERROR : Resolution not found 🔴");
	error_messages[ERROR_RES_LEN] = strdup("ERROR : Poor resolution definition 🔴");
	error_messages[ERROR_RES_VALUE] = strdup("ERROR : Resolution value below 0 🔴");
	error_messages[ERROR_AMB] = strdup("ERROR : Not ambient light found 🔴");
	error_messages[ERROR_FORMAT] = strdup("ERROR : File format not supported 🔴");
	error_messages[ERROR_OPEN] = strdup("ERROR : Error opening the file 🔴");
	error_messages[ERROR_CLOSE] = strdup("ERROR : Error closing the file 🔴");
	error_messages[ERROR_CAMERA] = strdup("ERROR : Invalid camera definition 🔴");
	if (error != 0 && error < ERROR_COUNT)
	{
		ft_putendl_fd(error_messages[error], STDERR_FILENO);
		for (int i = 0; i < ERROR_COUNT; i++)
		{
			free(error_messages[i]);
		}
		exit(EXIT_FAILURE);
	}
	else if (error == 0)
	{
		ft_printf(error_messages[error]);
		for (int i = 0; i < ERROR_COUNT; i++)
		{
			free(error_messages[i]);
		}
		exit(EXIT_SUCCESS);
	}
}

int	pre_parse(char *file)
{
	char	*line;
	char	**full_map;
	t_world	*world;
	int		count_line;
	int		fd;
	int		err;
	int		i;
	char	**data;
	int		res;

	count_line = 0;
	line = NULL;
	full_map = NULL;
	fd = open_scene_file(file);
	if (fd < 0)
		msg_exit(ERROR_OPEN);
	while ((line = get_next_line(fd)))
	{
		if (*line != '#' && *line != '\n')
		{
			count_line++;
		}
		free(line);
	}
	if (close(fd) < 0)
		msg_exit(ERROR_OPEN);
	full_map = (char **)malloc(sizeof(char *) * (count_line + 1));
	fd = open_scene_file(file);
	i = 0;
	while ((line = get_next_line(fd)))
	{
		if (*line != '#' && *line != '\n')
		{
			full_map[i] = ft_strdup(line);
			i++;
		}
		free(line);
	}
	full_map[i] = '\0';
	i = -1;
	data = NULL;
	res = 0;
	while (full_map[++i])
	{
		data = ft_split_space(full_map[i]);
		if (!ft_strcmp(data[0], "R"))
		{
			if (double_pointer_len(data) != 3)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_RES_LEN);
			}
			else if (ft_atoi(data[1]) <= 0 || ft_atoi(data[2]) <= 0)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_RES_VALUE);
			}
		}
		else if (!ft_strcmp(data[0], "A"))
		{
			if (double_pointer_len(data) != 3)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_RES_LEN);
			}
			else if (ft_atof(data[1]) > 1 || ft_atoc(data[2]) < 0)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_BRIGHTNESS);
			}
		}
		else if (!ft_strcmp(data[0], "c"))
		{
			if (double_pointer_len(data) != 4)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_CAMERA);
			}
			else if (ft_atof(data[3]) < 0 || ft_atof(data[3]) > 180)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_CAMERA);
			}
		}
		else if (!ft_strcmp(data[0], "l"))
		{
			if (double_pointer_len(data) != 4)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_FORMAT);
			}
			else if (ft_atof(data[2]) < 0 || ft_atof(data[2]) >= 1)
			{
				printf("data[2] = %f\n", ft_atof(data[2]));
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_BRIGHTNESS);
			}
			else if (ft_atoc(data[3]) == -1)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_COLOR);
			}
		}
		else if (!ft_strcmp(data[0], "pl"))
		{
			if (double_pointer_len(data) != 4)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_FORMAT);
			}
			else if (ft_atoc(data[3]) == -1)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_COLOR);
			}
		}
		else if (!ft_strcmp(data[0], "sp"))
		{
			if (double_pointer_len(data) < 4)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_PARSE);
			}
			else if (ft_atoc(data[3]) == -1)
			{
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_COLOR);
			}
			else if (double_pointer_len(data) > 4 && ft_strcmp(data[4], "GLASS"))
			{
				printf("data[4] = %s\n", data[4]);
				free_double(data);
				free_double(full_map);
				msg_exit(ERROR_FORMAT);
				
			}
		}
		free_double(data);
	}
	// free_double(data);
	free_double(full_map);
	// msg_exit(ERROR_AMB);
	return (res);
}

int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		msg_exit(ERROR_ARG);
	if (pre_parse(argv[1]) != 0)
		msg_exit(ERROR_FORMAT);
	// world = scene_init(argv[1]);
	// server = new_server(world);
	// if (!server)
	// 	full_message_exit(ERROR_SERVER, world, server);
	// render(server);
	// init_texture(server->mlx, server->world->texture_img,
		// "/home/francfer/Desktop/TEST/textures/brick.xpm");
	// mlx_events(server);
	return (EXIT_SUCCESS);
}
