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
// char	**create_error(void)
// {
// 	char	*error_messages[ERROR_COUNT];

// 	error_messages[ERROR_JEFE] = strdup("🐷 Nos vemos jefe 🐷");
// 	error_messages[ERROR_MALLOC] = strdup("ERROR: Malloc problem 🔴");
// 	error_messages[ERROR_PARSE] = strdup("ERROR: While parsing 🔴");
// 	error_messages[ERROR_CREATE] = strdup("ERROR: return (pthread_create() 🔴");
// 	error_messages[ERROR_JOIN] = strdup("ERROR: return (pthread_join() 🔴");
// 	error_messages[ERROR_COLOR] = strdup("ERROR: Invalid color definition 🔴");
// 	error_messages[ERROR_SERVER] = strdup("ERROR: Can't create server 🔴");
// 	error_messages[ERROR_COL_VAL] = strdup("ERROR: Color value out range 🔴");
// 	error_messages[ERROR_VECTOR] = strdup("ERROR: Vector out range 🔴");
// 	error_messages[ERROR_BRIGHTNESS] = strdup("ERROR: Bright out of range 🔴");
// 	error_messages[ERROR_RATIO] = strdup("ERROR: Light ratio out of range 🔴");
// 	error_messages[ERROR_DIR] = strdup("ERROR: Invalid number detected 🔴");
// 	error_messages[ERROR_ARG] = strdup("ERROR: Bad number of arguments 🔴");
// 	error_messages[ERROR_ID] = strdup("ERROR: Invalid object's identifier 🔴");
// 	error_messages[ERROR_RES] = strdup("ERROR: Resolution not found 🔴");
// 	error_messages[ERROR_RES_LEN] = strdup("ERROR: Resolution 🔴");
// 	error_messages[ERROR_RES_VALUE] = strdup("ERROR: Resolution below 0 🔴");
// 	error_messages[ERROR_AMB] = strdup("ERROR: Not ambient light found 🔴");
// 	error_messages[ERROR_FORMAT] = strdup("ERROR: File format not supported 🔴");
// 	error_messages[ERROR_OPEN] = strdup("ERROR: Error opening the file 🔴");
// 	error_messages[ERROR_CLOSE] = strdup("ERROR: Error closing the file 🔴");
// 	error_messages[ERROR_CAMERA] = strdup("ERROR: Invalid camera definition 🔴");
// 	return (error_messages);
// }

char	**create_error(void)
{
	char	**error_messages;

	error_messages = malloc(ERROR_COUNT * sizeof(char *));
	if (error_messages == NULL)
		return (NULL);
	error_messages[ERROR_JEFE] = strdup("🐷 Nos vemos jefe 🐷");
	error_messages[ERROR_PARSE] = strdup("ERROR: While parsing 🔴");
	error_messages[ERROR_CREATE] = strdup("ERROR: return (pthread_create() 🔴");
	error_messages[ERROR_JOIN] = strdup("ERROR: return (pthread_join() 🔴");
	error_messages[ERROR_COLOR] = strdup("ERROR: Invalid color definition 🔴");
	error_messages[ERROR_BRIGHTNESS] = strdup("ERROR: Bright out of range 🔴");
	error_messages[ERROR_DIR] = strdup("ERROR: Invalid number detected 🔴");
	error_messages[ERROR_ARG] = strdup("ERROR: Bad number of arguments 🔴");
	error_messages[ERROR_ID] = strdup("ERROR: Invalid object's identifier 🔴");
	error_messages[ERROR_RES] = strdup("ERROR: Resolution not found 🔴");
	error_messages[ERROR_RES_LEN] = strdup("ERROR: Resolution 🔴");
	error_messages[ERROR_RES_VALUE] = strdup("ERROR: Resolution below 0 🔴");
	error_messages[ERROR_AMB] = strdup("ERROR: Not ambient light found 🔴");
	error_messages[ERROR_FORMAT] = strdup("ERROR: File format not supported 🔴");
	error_messages[ERROR_OPEN] = strdup("ERROR: Error opening the file 🔴");
	error_messages[ERROR_CLOSE] = strdup("ERROR: Error closing the file 🔴");
	error_messages[ERROR_CAMERA] = strdup("ERROR: Invalid camera definition 🔴");
	return (error_messages);
}

void	msg_exit(ErrorType error)
{
	char	**error_messages;
	int		i;

	i = 0;
	error_messages = create_error();
	if (error != 0 && error < ERROR_COUNT)
	{
		ft_putendl_fd(error_messages[error], STDERR_FILENO);
		while (i < ERROR_COUNT)
			free(error_messages[i++]);
		free(error_messages);
		exit(EXIT_FAILURE);
	}
	else if (error == 0)
	{
		ft_printf(error_messages[error]);
		while (i < ERROR_COUNT)
			free(error_messages[i++]);
		free(error_messages);
		exit(EXIT_SUCCESS);
	}
}

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
	else if (ft_atoc(data[6]) == -1 || ft_atoc(data[6]) == -1)
		return (ERROR_COLOR);
	return (0);
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
		if (!ft_strcmp(data[0], "R") && check_r(data) != 0)
			res = check_r(data);
		else if (!ft_strcmp(data[0], "A") && check_a(data) != 0)
			res = check_a(data);
		else if (!ft_strcmp(data[0], "c") && check_c(data) != 0)
			res = check_c(data);
		else if (!ft_strcmp(data[0], "l") && check_l(data) != 0)
			res = check_l(data);
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
	// free_double(full_map);
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

int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		msg_exit(ERROR_ARG);
	if (pre_parse(argv[1]) != 0)
		msg_exit(pre_parse(argv[1]));
	// world = scene_init(argv[1]);
	// server = new_server(world);
	// if (!server)
	// 	full_message_exit(ERROR_SERVER, world, server);
	// render(server);
	// init_texture(server->mlx, server->world->texture_img,
	// 	"./textures/brick.xpm");
	// mlx_events(server);
	return (EXIT_SUCCESS);
}
