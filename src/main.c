#include <minirt.h>

// Función principal del proyecto
// Lo primero que hacemos es verificar que el número de argumentos es válido,
// es decir, si el número de argumentos es menor que 2 o mayor que 3 paramos la
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
// char	**create_error(void)
// {
// 	char	*error_messages[ERROR_COUNT];

// 	error_messages[ERROR_JEFE] = strdup("🐷 Nos vemos jefe 🐷");
// 	error_messages[ERROR_MALLOC] = strdup("ERROR: Malloc problem 🔴");
// 	error_messages[ERROR_PARSE] = strdup("ERROR: While parsing 🔴");
// 	error_messages[ERROR_CREATE] = strdup("ERROR: return (pthread_create() 🔴");
// 	error_messages[ERROR_JOIN] = strdup("ERROR: return (pthread_join() 🔴");
// 	error_messages[ERROR_COLOR] = strdup("ERROR: Invalid color definition 🔴");
// 	error_messages[ERROR_SERVER] = strdup("ERROR: Can't create server 🔴");
// 	error_messages[ERROR_COL_VAL] = strdup("ERROR: Color value out range 🔴");
// 	error_messages[ERROR_VECTOR] = strdup("ERROR: Vector out range 🔴");
// 	error_messages[ERROR_BRIGHTNESS] = strdup("ERROR: Bright out of range 🔴");
// 	error_messages[ERROR_RATIO] = strdup("ERROR: Light ratio out of range 🔴");
// 	error_messages[ERROR_DIR] = strdup("ERROR: Invalid number detected 🔴");
// 	error_messages[ERROR_ARG] = strdup("ERROR: Bad number of arguments 🔴");
// 	error_messages[ERROR_ID] = strdup("ERROR: Invalid object's identifier 🔴");
// 	error_messages[ERROR_RES] = strdup("ERROR: Resolution not found 🔴");
// 	error_messages[ERROR_RES_LEN] = strdup("ERROR: Resolution 🔴");
// 	error_messages[ERROR_RES_VALUE] = strdup("ERROR: Resolution below 0 🔴");
// 	error_messages[ERROR_AMB] = strdup("ERROR: Not ambient light found 🔴");
// 	error_messages[ERROR_FORMAT] = strdup("ERROR: File format not supported 🔴");
// 	error_messages[ERROR_OPEN] = strdup("ERROR: Error opening the file 🔴");
// 	error_messages[ERROR_CLOSE] = strdup("ERROR: Error closing the file 🔴");
// 	error_messages[ERROR_CAMERA] = strdup("ERROR: Invalid camera definition 🔴");
// 	return (error_messages);
// }





int	main(int args, char **argv)
{
	t_world		*world;
	t_server	*server;

	if (args < 2 || args > 3)
		msg_exit(ERROR_ARG);
	if (pre_parse(argv[1]) != 0)
		msg_exit(pre_parse(argv[1]));
	// world = scene_init(argv[1]);
	// server = new_server(world);
	// if (!server)
	// 	full_message_exit(ERROR_SERVER, world, server);
	// render(server);
	// init_texture(server->mlx, server->world->texture_img,
	// 	"./textures/brick.xpm");
	// mlx_events(server);
	return (EXIT_SUCCESS);
}
