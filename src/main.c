#include <minirt.h>

void	apply_stuff(t_ray *ray, t_world *world, int depth)
{
	apply_texture(ray, world);
	apply_material(ray, world, depth);
	apply_checkerboard(ray, world);
}

void	check_server(t_server *server, int *t, int *rows_per_thread)
{
	*rows_per_thread = server->height / NUM_THREADS;
	if (!server->world->cameras)
		return ;
	*t = -1;
}

int	exit_hook(t_server *server)
{
	free_server(server);
	message_exit(ERROR_JEFE);
	return (0);
}

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
		"/home/francfer/Desktop/TEST/textures/wood.xpm");
	mlx_events(server);
	return (EXIT_SUCCESS);
}
