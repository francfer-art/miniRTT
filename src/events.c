#include <minirt.h>

// Función para cerrar y salir de la ventana de forma limpia
// Recibe como argumento el server, que es la estructura donde está toda la información. Dicha
// estructura se liberará antes de salir
int exit_hook(t_server *server)
{
    free_server(server);
    exit(EXIT_SUCCESS);
}

// Función para mantener la consistencia visual del contenido de la pantalla. Cuando la pantalla
// se  actualiza, esta función se encarga de que todo se muestre correctamente.
int expose_hook(t_server *server)
{
    return (mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0));
}

// Función para manejar los eventos en la escena
// Eventos que tengo que manejar:
// ESC para cerrar la escena --> exit_hook()
// C para cambiar a la siguiente cámara
// Flechas para modificar las coordenadas de la cámara para poder moverme por la
// escnea
int key_press_hook(int keycode, t_server *server)
{
    if (keycode == XK_Escape)
        exit_hook(server);
    else if (keycode == XK_c)
        change_camera(server, 1);
   /*  else if (keycode == XK_Left || keycode == XK_Right)
        move_camera(server, keycode);
    else if (keycode == XK_Up || keycode == XK_Down)
        move_camera(server, keycode); */
    else
        ;
    render(server);
    return (0);
}

// Función que maneja los eventos de la minilibX
// Almacenamos la ventana
// Registro el manejador para eventos de presión de teclas
// Registro el manejador para el evento de cierre de la ventana
// Registro el manejador para el evento de exposición de la ventana --> Esto es útil
// cuando la ventana se minimiza, expose_hook() se encargará de redibujar la 
// el contenido de la ventana
// Inicio el bucle de eventos
int mlx_events(t_server *server)
{
    void    *window;

    window = server->window;
    mlx_hook(window, KeyPress, KeyPressMask, key_press_hook, server);
    mlx_hook(window, DestroyNotify, StructureNotifyMask, exit_hook, server);
    mlx_expose_hook(window, expose_hook, server);
    mlx_loop(server->mlx);
    return (0);
}