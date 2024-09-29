#include <minirt.h>

// Función que actualiza el brillo de la luz ambiente. Siempre sumo el número, si el brillo
// resultante es negativo, el brillo será cero. Si el brillo es mayor que 1 el brillo será 1.
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

void    color_selec(t_server *server, char *pixels, int y, int x, int fd)
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char color[3];

    r = pixels[y * server->image->size_line + x * 4 + 2];
    g = pixels[y * server->image->size_line + x * 4 + 1];
    b = pixels[y * server->image->size_line + x * 4 + 0];
    color[0] = b;
    color[1] = g;
    color[2] = r;
    write(fd, color, 3);
}

void    put_pixels(t_server *server, char *pixels, int fd)
{
    int             padding;
    unsigned char   pad[3];
    int             y;
    int             x;

    padding = (4 - (server->width * 3) % 4) % 4;
    y = server->height - 1;
    while (y >= 0)
    {
        x = -1;
        while (++x < server->width)
            color_selec(server, pixels, y, x, fd);
        if (padding > 0)
        {
            pad[0] = 0;
            pad[1] = 0;     
            pad[2] = 0;
            write(fd, pad, padding);
        }
        y--;
    }
}

void take_screenshot(t_server *server)
{
    static int  screenshot_counter = 0;
    char        *filename;
    char        *number;
    char        *pixels;
    int         fd;

    create_filename(&number, &screenshot_counter, &filename);
    pixels = mlx_get_data_addr(server->image->image, &server->image->bpp, &server->image->size_line, &server->image->endian);
    fd  = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd < 0)
        return;
    free(filename);
    set_header_info(server, fd);
    put_pixels(server, pixels, fd);
    printf("%s Screenshot taken 📸%s\n", PURPLE, RESET);
    close(fd);
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
    mlx_loop(server->mlx);
    return (0);
}