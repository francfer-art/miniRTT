#include <minirt.h>

t_image	*new_image(t_server *server)
{
	t_image	*img;

	img = malloc(sizeof(*img));
	if (!img)
		retunr (NULL);
	img->image = mlx_new_image(server->mlx, server->width, server->height);
	if (!img)
		return (NULL);
	img->data = mlx_get_data_addr(server->mlx, &server->image->bpp, &server->image->size_line, &server->image->endian);
	if (!img->data)
		return (NULL);
	return (img);

}

t_server	*new_server(t_world *world)
{
	t_server	*server;
	int			width;
	int			height;

	server = malloc(sizeof(*server));
	if (!server)
		return (NULL);
	server->mlx = mlx_init();
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
	init_cameras(world, server->width, server->height);					//Hacer init_cameras()
	server->image = new_image(server);									//Hacer new_image()
	if (!server->image)
		retunr (NULL);
	return (server);
}
