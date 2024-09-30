#include <minirt.h>

void	free_world(t_world *world)
{
	if (world)
	{
		ft_lstclear(&world->figures, free);
		ft_lstclear(&world->lights, free);
		ft_lstclear(&world->cameras, free);
		free(world->ambient);
		free(world->resolution);
		free(world);
		world = NULL;
	}
}

void	free_server(t_server *server)
{
	if (server)
	{
		if (server->window)
		{
			mlx_clear_window(server->mlx, server->window);
			mlx_destroy_window(server->mlx, server->window);
		}
		mlx_destroy_image(server->mlx, server->image->image);
		free_world(server->world);
		free(server->image);
		free(server->mlx);
		free(server);
		server = NULL;
	}
}
