#include <minirt.h>

// Función que rellanará un cuadrado de pixeles en lugar de 1 solo. EStá función es la 
// utilizada en la función render_low. 
void    fill_pixels(t_server *server, int scale_factor, int i, int j, t_color pixel_color)
{
    int x;
    int y;
    int orig_i;
    int orig_j;

    y = -1;
    while (++y < scale_factor)
    {
        x = -1;
        while (++x < scale_factor)
        {
            orig_i = i * scale_factor + x;
            orig_j = j * scale_factor + y;
            my_put_pixel(server, orig_i, server->height - 1 - orig_j, pixel_color);
        }
    }
}

int	adjust_scale_factor(t_server *server)
{
    int	area;
	int	scale_factor;

    area = server->width * server->height;
	scale_factor = area / BASE_FACTOR;
    if (scale_factor < 1)
        scale_factor = 5;
    return scale_factor;
}

void *render_section_super(void *threadarg)
{
    t_thread_data   *data;
    int             j;

    data = (t_thread_data *)threadarg;
    j = data->start_row;
    while (j < data->end_row)
    {
        render_row(data, j);
        j++;
    }
    pthread_exit(NULL);
}


void render_row(t_thread_data *data, int j)
{
    int i;

    i = 0;
    while (i < data->server->width)
    {
        render_pixel(data, i, j);
        i++;
    }
}

void    join_menu_image(pthread_t *threads, t_server *server)
{
    int t;

    t = -1;
    while (++t < NUM_THREADS)
    {
        if (pthread_join(threads[t], NULL))
            full_message_exit(ERROR_JOIN, NULL, server);
    }
    show_menu(server);
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}