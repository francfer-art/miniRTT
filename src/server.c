#include <minirt.h>

t_image	*new_image(t_server *server)
{
	t_image	*img;

	img = malloc(sizeof(*img));
	if (!img)
		return (NULL);
	img->image = mlx_new_image(server->mlx, server->width, server->height);
	if (!img)
		return (NULL);
	img->data = mlx_get_data_addr(server->mlx, &(server->image->bpp), &(server->image->size_line), &(server->image->endian));
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
	init_cameras(world, server->width, server->height);
	server->image = new_image(server);
	if (!server->image)
		retunr (NULL);
	return (server);
}

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
