#include <minirt.h>

int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		message_exit(ERROR_ARG);
	world = scene_init(argv[1]);
	server = new_server(world);
	if (!server)
		full_message_exit(ERROR_SERVER, world, server);
	render(server);
	init_texture(server->mlx, server->world->texture_img,
		"./textures/wood.xpm");
	mlx_events(server);
	return (EXIT_SUCCESS);
}
