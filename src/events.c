#include <minirt.h>

// Función para cerrar y salir de la ventana de forma limpia
// Recibe como argumento el server, que es la estructura donde está toda la información. Dicha
// estructura se liberará antes de salir
int exit_hook(t_server *server)
{
    free_server(server);
    message_exit(ERROR_JEFE);
    return (0);
}

// Función para mantener la consistencia visual del contenido de la pantalla. Cuando la pantalla
// se  actualiza, esta función se encarga de que todo se muestre correctamente.
int expose_hook(t_server *server)
{
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
    return (1);
}

void    update_hook(int keycode, t_server *server)
{
    if (keycode == XK_r || keycode == XK_g || keycode == XK_b || keycode == XK_t || keycode == XK_h || keycode == XK_n)
        update_ambient_color(server, keycode);
    else if (keycode == XK_CHECKERBOARD)
        update_checkerboard(server);
    else if (keycode == XK_m)
        update_material(server);
    else if (keycode == XK_TEXTURE)
        update_texture(server);
    else if (keycode == XK_PHONG)
        update_phong(server);
    else if (keycode == XK_BUMP)
        update_bump(server);
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
        full_message_exit(ERROR_JEFE, NULL, server);
    else if (keycode == XK_c || keycode == XK_x)
        change_camera(server, keycode);
    else if (keycode == XK_Left || keycode == XK_Right)
        move_camera_rotate(server, keycode);
    else if (keycode == XK_Up || keycode == XK_Down)
        move_camera_rotate(server, keycode);
    else if (keycode == XK_w || keycode == XK_s || keycode == XK_a || keycode == XK_d)
        move_camera_position(server, keycode);
    update_hook();
    else if (keycode == XK_SCREENSHOT)
        return (take_screenshot(server), 1);
    else if (keycode == XK_space)
    {
        render(server);
        return (1);
    }
    else
        return(1);
    render_low(server);
    return (0);
}

// Función encargada de manejar los eventos del ratón. La rueda del ratón hace zoom. De momento
// el botón izquierdo aumenta el brillo de la luz ambiente, y el botón derecho lo disminuye.
// Estoy pensando en utilizar dichos botones para seleccionar un objeto para poder moverlo por 
// la escena.
int mouse_handler(int button, int x, int y, t_server *server)
{
    x++;
    y++;
    if (button == XK_ZOOM_OUT)
        move_camera(server, button);
    else if (button == XK_ZOOM_IN)
        move_camera(server, button);
    else if (button == XK_LEFT_BUTTON)
        update_ambient_brightness(server, 0.05);
    else if (button == XK_RIGHT_BUTTON)
        update_ambient_brightness(server, -0.05);
    render_low(server);
    return (0);
}
