#include <minirt.h>

// Función que abre el archivo pasado de argumento
// Por defecto inicializo el fd en -1
// Busco el caracter '.', para comparar el puntero devuelto por la función
// ft_strchr(), con el valor de EXTENSION
// SI la el puntero devuelto existe y son iguales, abrimos el archivo
// Guardamos el valor devuelto del open en fd y devolvemos fd
int	open_scene_file(char *file)
{
	int		fd;
	char	*extension;

	fd = -1;
	extension = ft_strchr(file, '.');
	if (extension && !ft_strcmp(EXTENSION, extension))
	{
		fd = open(file, O_RDONLY);
		return (fd);
	}
	return (fd);
}

// Función para crear un puntero a la estructura world
// No recibe argumentos, creamos el puntero que vamos a devolver, asignamos
// el puntero devuelto por el malloc a ese puntero.
// Si no existe el puntero devolvemos NULL, en otro caso, asignamos NULL
// a todos los puntero de la estructura
// Finalmente devolvemos el puntero a la estructura recién creada
t_world	*new_world(void)
{
	t_world	*world;

	world = malloc(sizeof(*world));
	if (!world)
		return (NULL);
	world->resolution = NULL;
	world->figures = NULL;
	world->lights = NULL;
	world->cameras = NULL;
	world->ambient = NULL;
	return (world);
}

// Función 
// Abrimos el archivo, si el archivo es negativo, mostramos un error y salimos
// Creamos una nueva escena (world)
// Parseamos el archivo y vamos rellenando la escena
// Cerramos el archivo
// Comprobamos si ha habido algún error parseando el mapa --> En ese caso liberamos
// el mundo
// Finalmente devolvemos la escena
t_world	*scene_init(char *file)
{
	int		fd;
	int		err;
	t_world	*world;

	fd = open_scene_file(file);
	if (fd < 0)
		message_exit(ERROR_OPEN);
	world = new_world();
	err = parser_file(fd, world);
	if (close(fd) < 0)
		message_exit (ERROR_CLOSE);
	if (err)
	{
		free_world(world);
		message_exit(ERROR_PARSE);
	}
	return (world);
}
