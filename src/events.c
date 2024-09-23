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

// Función para actualizar los valores de la luz ambiente. Básicamente me quedo con cada
// canal de color y voy aumentando o disminuyendo la proporción de dicho color. Pulsando 
// R/G/B aumento el color en cuestión hasta 255 y pulsando T/H/N disminuyo dicha proporción
// los colores. Estas teclas están al lado de las otras (XD) 
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

void    update_texture(t_server *server)
{
    if (!server->world->texture)
    {
        server->world->texture = 1;
        server->world->material = 0;
        server->world->checkerboard = 0;
    }else
    {
        server->world->texture = 0;
        server->world->material = 0;
        server->world->checkerboard = 0;
    }
}
void    update_material(t_server *server)
{
    if (!server->world->material)
        server->world->material = 1;
    else
        server->world->material = 0;
}

void    update_checkerboard(t_server *server)
{
    if (!server->world->checkerboard)
    {
        server->world->checkerboard = 1;
        /* server->world->texture = 0; */
    }
    else
        server->world->checkerboard = 0;
}

void    update_phong(t_server *server)
{
    if (!server->world->phong)
        server->world->phong = 1;
    else
        server->world->phong = 0;
}

void    update_bump(t_server *server)
{
    if (!server->world->bump && server->world->texture)
        server->world->bump = 1;
    else
        server->world->bump = 0;
}

void    create_filename(char **number, int *screenshot_counter, char **filename)
{
    *filename = malloc(sizeof(char *));
    if (!*filename)
        return ;
    *number = ft_itoa(*screenshot_counter);
    ft_strcpy(*filename, "save/screenshot_");
    ft_strcat(*filename, *number);
    ft_strcat(*filename, ".bmp");
    free(*number);
    (*screenshot_counter)++;
}


void    set_info(t_server *server, unsigned char bmp_info_header[40])
{
    bmp_info_header[0] = 40;
    bmp_info_header[1] = 0;
    bmp_info_header[2] = 0;
    bmp_info_header[3] = 0;
    bmp_info_header[4] = (unsigned char)(server->width);
    bmp_info_header[5] = (unsigned char)(server->width >> 8);
    bmp_info_header[6] = (unsigned char)(server->width >> 16);
    bmp_info_header[7] = (unsigned char)(server->width >> 24);
    bmp_info_header[8] = (unsigned char)(server->height);
    bmp_info_header[9] = (unsigned char)(server->height >> 8);
    bmp_info_header[10] = (unsigned char)(server->height >> 16);
    bmp_info_header[11] = (unsigned char)(server->height >> 24);
    bmp_info_header[12] = 1;
    bmp_info_header[13] = 0;
    bmp_info_header[14] = 24;
    bmp_info_header[15] = 0;
}

void    set_file(int file_size, unsigned char bmp_file_header[14])
{
    bmp_file_header[0] = 'B';
    bmp_file_header[1] = 'M';
    bmp_file_header[2] = (unsigned char)(file_size);
    bmp_file_header[3] = (unsigned char)(file_size >> 8);
    bmp_file_header[4] = (unsigned char)(file_size >> 16);
    bmp_file_header[5] = (unsigned char)(file_size >> 24);
    bmp_file_header[6] = 0;
    bmp_file_header[7] = 0;
    bmp_file_header[8] = 0;
    bmp_file_header[9] = 0;
    bmp_file_header[10] = 54;
    bmp_file_header[11] = 0;
    bmp_file_header[12] = 0;
    bmp_file_header[13] = 0;
}
void    set_header_info(t_server *server, int fd)
{     
    int file_size;
    unsigned char *bmp_file_header;
    unsigned char *bmp_info_header;

    bmp_file_header = malloc(14);
    if (!bmp_file_header)
        return;
    bmp_info_header = malloc(40);
    if (!bmp_info_header)
        return;
    file_size = 454 + (3 * server->width * server->height);
    set_file(file_size, bmp_file_header);
    set_info(server, bmp_info_header);
    write(fd, bmp_file_header, 14);
    write(fd, bmp_info_header, 40);
    free(bmp_file_header);
    free(bmp_info_header);
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
/* void take_screenshot(t_server *server)
{
    static int  screenshot_counter = 0;
    char        *filename;
    char        *number;
    char        *pixels;

    create_filename(&number, &screenshot_counter, &filename);
    pixels = mlx_get_data_addr(server->image->image, &server->image->bpp, &server->image->size_line, &server->image->endian);
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd < 0)
        return ;
    free(filename);
    int file_size = 54 + (3 * server->width * server->height);
    unsigned char bmp_file_header[14] = {
        'B', 'M',
        (unsigned char)(file_size), (unsigned char)(file_size >> 8), (unsigned char)(file_size >> 16), (unsigned char)(file_size >> 24),
        0, 0, 0, 0,
        54, 0, 0, 0
    };     write(fd, pad, padding);
    int padding = (4 - (server->width * 3) % 4) % 4;

    for (int y = server->height - 1; y >= 0; y--)
    {
        for (int x = 0; x < server->width; x++)
        {
            unsigned char r = pixels[y * server->image->size_line + x * 4 + 2];
            unsigned char g = pixels[y * server->image->size_line + x * 4 + 1];
            unsigned char b = pixels[y * server->image->size_line + x * 4 + 0];

            unsigned char color[3] = {b, g, r};
            write(fd, color, 3);
        }

        if (padding > 0)
        {
            unsigned char pad[3] = {0, 0, 0};
            write(fd, pad, padding);
        }
    }

    printf("%s Screenshot taken 📸%s\n", PURPLE, RESET);
    close(fd);
} */

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