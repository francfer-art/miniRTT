#include <minirt.h>

int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		msg_exit(ERROR_ARG);
	if (pre_parse(argv[1]) != 0)
		msg_exit(pre_parse(argv[1]));
	world = scene_init(argv[1]);
	server = new_server(world);
	if (!server)
		full_message_exit(ERROR_SERVER, world, server);
	render(server);
	init_texture(server->mlx, server->world->texture_img, "/home/francfer/Desktop/TEST/textures/brick.xpm");
	mlx_events(server);
	return (EXIT_SUCCESS);
}
