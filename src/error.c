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
		ft_putstr_fd(msg, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	exit(EXIT_FAILURE);
}
