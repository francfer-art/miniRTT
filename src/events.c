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

int key_press_hook(int keycode, t_server *server)
{
    //Funcion para manejar los eventos en la escena
    //Eventos que tengo que manejar
    // ESC para cerrar la escena --> exit_hook()
    // C para cambiar a la siguiente camara
    // Flechas para modificar las coordenadas de la camara
    //  para poder moverme por la escnea
    if (keycode == 0Xff1b) //Escape
        exit_hook(server);
    else if (keycode == 0x0043) //C para cambiar de camara
        change_camera(server, 1);
    else if (keycode == 0xff51 || keycode == 0xff53) //Arrows para mover la camara horizontalmente
        move_camera(server, keycode);
    else if (keycode == 0xff52 || keycode == 0xff54)//Arrows para mover la camra verticalmente
        move_camera(server, keycode);
    render(server);
    return (0);
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