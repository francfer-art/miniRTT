#include <minirt.h>

// función que muestra un mensaje por pantalla y sale de forma exitosa
// Esta función está bien pero hay que cambiarla para que se le pase como 
// argumento un puntero a t_world y un puntero a t_server, para llamar a las
// funciones que liberan dichos punteros.
// Si se le pasa NULL como argumento no liberarán dichos punteros
void	message_exit(char *msg)
{
	if (msg)
	{
		ft_printf("%s\n", msg);
	}
	exit(EXIT_FAILURE);
}

// Esta función es la que limpia todo y muestra un error, en caso de que queramos salir de 
// alguna función con un mensaje de error definido en minirt.h, podemos llamar a esta
// función
void	full_message_exit(char *msg, t_world *world, t_server *server)
{
	free_world(world);
	free_server(server);
	msg_exit(msg);
}
