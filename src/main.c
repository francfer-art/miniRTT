#include <minirt.h>

// Función principal del proyecto
// Lo primero que hacemos es verificar que el número de argumentos es válido,
// es decir, si el número de argumentos es menor que 2 o mayor que 3 paramos la 
// ejecucuión aquí.
// Inicializamos el mundo --> Esta función leerá el mapa y parseará toda la
// información e irá creando las figuras y guardandolas en la estructura world
// Creamos el server --> Esta función creará los punteros necesarios para
// que la comunicación con la minilibX sea correcta
// Si falla la creación del server --> Mensaje de error
// Renderizamos la escena con toda la información
// Llamamos la función que manejará los eventos --> El programa acabará de forma
// limpia cuando se pulse la tecla ESC o cuando cerremos la ventana
int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		message_exit(ERROR_ARG);
	world = scene_init(argv[1]);
	server = new_server(world);
	if (!server)
		message_exit(ERROR_SERVER);
	render(server);
	mlx_events(server);
	return (EXIT_SUCCESS);
}
