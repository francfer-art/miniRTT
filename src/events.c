#include <minirt.h>

int exit_hook(t_server *server)
{
    free_server(server);
    exit(EXIT_SUCCESS);
}

int expose_hook(t_server *server)
{
    return (mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0));
}

int key_press_hook(int keycode, void *param)
{
    //Funcion para manejar los eventos en la escena
    //Eventos que tengo que manejar
    // ESC para cerrar la escena --> exit_hook()
    // C para cambiar a la siguiente camara
    // Flechas para modificar las coordenadas de la camara
    //  para poder moverme por la escnea
    if (keycode == )
}

void    mlx_events(t_server *server)
{
    void    *window;

    window = server->window;
    mlx_hook(window, KeyPressMask, key_press_hook, server);
    mlx_hook(window, DestroyNotify, exit_hook, server);
    mlx_expose_hook(window, expose_hook, server);
    mlx_loop(server->mlx);
    return (0);
}