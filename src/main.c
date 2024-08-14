#include <minirt.h>

int	main(int args, char **argv)
{
	int			status;
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		message_exit(ERROR_ARG);
	world = scene_init(argv[1]);
	server = new_server(world);
	if (!server)
		message_exit(ERROR_SERVER);
	render(server);
	mlx_listen(server);
	return (EXIT_SUCCESS);
}
