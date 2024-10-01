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
