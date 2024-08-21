#include <minirt.h>

// Función que crea una nueva imagen y devuelve un puntero a dicha imagen
// Recibe un puntero a la estructura t_server, para tener los argumentos necesarios
// requeridos por mlx_new_image o mlx_get_data_addr
// Alojamos dinámicamente la memoria necesaria para el puntero de la imagen
// Asignamos el campo imagen a la estuctura con la función mlx_new_image
// Asignamos el campo data a la estructura con la función mlx_get_data_addr
// Devolvemos el puntero a la estructura que acabamos de alojar.
// Si alguna de las llamadas a las funciones de la mlx falla, liberamos el puntero
// y devolvemos NULL
t_image	*new_image(t_server *server)
{
	t_image	*img;

	img = malloc(sizeof(*img));
	if (!img)
		return (NULL);
	img->image = mlx_new_image(server->mlx, server->width, server->height);
	if (!img)
		return (free(img), NULL);
	img->data = mlx_get_data_addr(server->mlx, &(server->image->bpp), &(server->image->size_line), &(server->image->endian));
	if (!img->data)
		return (free(img), NULL);
	return (img);
}

// Función que crea e inicializa un puntero a una estructura t_server
// Lo primero que hacemos es alojar dinámicamente el puntero a la estructura
// Inicializamos la conexión a la mlx con mlx_init()
// Obtenemos el tamaño de la pantalla y lo guardamos en width y height
// Comprobamos la resolución introducida, si la resolución deseada es mayor que la de la pantalla
// ajustamos dicha resolución que no exceda los límites de la pantalla
// Se crea una nueva ventana con las dimensiones ajustadas y un título definido
// Inicializamos los campos del servidor, inicializamos las cámaras, y creamos la imagen del servidor
// Finalmente devolvemos el servidor
// PD : Si alguna llamada falla, liberamos el server, el world y devolvemos NULL
t_server	*new_server(t_world *world)
{
	t_server	*server;
	int			width;
	int			height;

	server = malloc(sizeof(*server));
	if (!server)
		return (NULL);
	server->mlx = mlx_init();
	if (!(server->mlx))
		return (NULL);
	mlx_get_screen_size(server->mlx, &width, &height);
	if (world->resolution[0] > width)
		world->resolution[0] = width;
	if (world->resolution[1] > height)
		world->resolution[1] = height;
	server->window = mlx_new_window(server->mlx, server->width, server->height, WINDOW_TITLE);
	if (!server->window)
		return (NULL);
	server->world = world;
	server->width = world->resolution[0];
	server->height = world->resolution[1];
	init_cameras(world->cameras, server->width, server->height);
	server->image = new_image(server);
	if (!server->image)
		return (NULL);
	return (server);
}

// Función que coloca un pixel de un color específico en una posición (x,y)
// Lo primero es calcular la cantidad de bytes por pixel, para eso dividemos bpp
// entre 8 para pasar de bits por pixel a bytes por pixel
// Ahora calculamos la dirección de memoria donde se colocará el pixel
// Calculamos el desplazamiento en bytes para llegar a la posición x e y, sumamos
// estos desplazamientos con dirección donde empieza el puntero de la imagen
// Covertimos el color a un valor hexadecimal 
// Ft_memcpy escribe opp bytes desde &hex_color hacia color_add, esto escribe el 
// color en la memoria
void	my_put_pixel(t_server *server, int x, int y, t_color color)
{
	char	*color_add;
	int		hex_color;
	int		opp;

	opp = server->image->bpp / 8;
	color_add = (server->image->data + y * server->image->size_line + x * opp);
	hex_color = mlx_get_color_value(server->mlx, color);
	ft_memcpy(color_add, &hex_color, opp);
}
