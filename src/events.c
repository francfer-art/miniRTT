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

void update_ambient_color(t_server *server, int code)
{
    int color;
    int red;
    int green;
    int blue;

    color = server->world->ambient->color;
    red = (color >> 0x010) & 0xFF;
    green = (color >> 0x08) & 0xFF;
    blue = color & 0xFF;
    if (code == XK_r && red <= 250)
        red = red + 5;
    else if (code == XK_t && red >= 5)
        red = red - 5;
    else if (code == XK_g && green <= 250)
        green = green + 5;
    else if (code == XK_h && green >= 5)
        green = green - 5;
    else if (code == XK_b && blue <= 250)
        blue = blue + 5;
    else if (code == XK_n && blue >= 5)
        blue = blue - 5;
    color = (red << 0x010) | (green << 0x08) | blue;
    server->world->ambient->color = color;
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
    else if (keycode == XK_r || keycode == XK_g || keycode == XK_b || keycode == XK_t || keycode == XK_h || keycode == XK_n)
        update_ambient_color(server, keycode);
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

void    update_ambient_brightness(t_server *server, float num)
{
    float   brightness;

    server->world->ambient->brightness += num;
    brightness = server->world->ambient->brightness;
    if (brightness < 0)
        brightness = 0;
    else if (brightness > 1)
        brightness = 1;
}

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
    mlx_hook(window, ButtonPress, ButtonPressMask, mouse_handler, server);
    //mlx_expose_hook(window, expose_hook, server);
    mlx_loop(server->mlx);
    return (0);
}