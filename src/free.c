#include <minirt.h>

// Función para liberar la memoria dinámica de la estructura world
// Limpiamos la lista de figuras, la lista de luces y la lista de cámaras
// Liberamos la luz ambiente, la resolucion y la estructura de world
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

// función para liberar la memoria dinámica de la estructura server
// Primero chequeamos que exista la ventana, en caso afirmativo, limpiamos
// la ventana y la destruimos
// Ahora destruimos la imagen, liberamos el world, la image, el puntero 
// a la mlx y la estructura server
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
