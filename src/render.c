#include <minirt.h>

// Devolverá 1 si hay alguna intersección con algún objeto de la escena,
// y devolverá 0 si no hay intersección. Recibe como argumento un rayo y una lista
// enlazada donde cada nodo es una figura
// Lo primero que hacemos es inicializar los valores de hit en 0 y la r en INFINITO
// para asegurarnos de que la intersección encontrada sea la más cercana.
// Iteramos mientras exista nuestra lista enlazada. En cada iteración obtenemos el
// tipo de la figura y llamamos a la función de intersección correspondiente
// Cuando la función acabe se devuelve 1 o 0, lo importante es que mientras vamos 
// iterando la estructura rayo se actualiza si hay intersección y si la distancia
// a dicha intersección es más cercana de la que había antes. Por eso es importante
// inicializar la distancia t a INFINITO al principio del bucle.
int intersec(t_ray *ray, t_list *figures)
{
    int     hit;
    t_type  type;

    hit = 0;
    ray->record.t = INFINITY;
    while (figures)
    {
        type = *((t_type *)figures->content);
        if (type == CYLINDER)
            hit |= hit_cylinder(ray, (t_cylinder *)figures->content);
        else if (type == PLANE)
            hit |= hit_plane(ray, (t_plane *)figures->content);
        else if (type == SPHERE)
            hit |= hit_sphere(ray, (t_sphere *)figures->content);
        else if (type == SQUARE)
            hit |= hit_square(ray, (t_square *)figures->content);
        else if (type == TRIANGLE)
            hit |= hit_triangle(ray, (t_triangle *)figures->content);
        figures = figures->next;
    }
    return (hit);
}

// Función que realiza el render en una sección delimitada. Dicha sección la realiza
// un hilo diferente. La idea es dividir la ventana en tantos cuadrados como hilos
// tenga el procesador
void *render_section(void *threadarg)
{
    t_thread_data   *data;
    int             i;
    int             j;
    t_color         pixel_color;
    t_ray           ray;
    
    data = (t_thread_data *)threadarg;
    j = data->start_row;
    while (j < data->end_row)
    {
        i = 0;
        while (i < data->server->width)
        {
            ray = generate_ray(data->server->world->cameras->content, (float)i / data->server->width, (float)j / data->server->height);
            pixel_color = raytracer(&ray, data->server->world, 5);
            my_put_pixel(data->server, i, data->server->height - 1 - j, pixel_color);
            i++;
        }
        j++;
    }
    pthread_exit(NULL);
}

t_color average_color(t_color *colors, int num_colors)
{
    int r_sum;
    int g_sum;
    int b_sum;
    int i;

    r_sum = 0;
    g_sum = 0;
    b_sum = 0;
    i = -1;
    while(++i < num_colors)
    {
        r_sum += (colors[i] >> 16) & 0xFF;
        g_sum += (colors[i] >> 8) & 0xFF;
        b_sum += colors[i] & 0xFF;
    }
    return ((r_sum / num_colors) << 16) | ((g_sum / num_colors)) << 8 | (b_sum / num_colors);
}

/* void *render_section_super(void *threadarg)
{
    t_thread_data   *data;
    int             i, j, m, n;
    t_color         pixel_colors[4];
    t_color         sample_color;
    t_ray           ray;
    t_color averaged_color;

    data = (t_thread_data *)threadarg;
    j = data->start_row;
    while (j < data->end_row)
    {
        i = 0;
        while (i < data->server->width)
        {
            for (int k = 0; k < SAMPLES_PER_PIXEL; k++)
            {
                pixel_colors[k] = 0;
            }

            int k = 0;
            for (m = 0; m < SQRT_SAMPLES; m++)
            {
                for (n = 0; n < SQRT_SAMPLES; n++)
                {
                    float u;
                    float v;
                    u = ((float)i + (m + 0.5) / SQRT_SAMPLES) / data->server->width;
                    v = ((float)j + (n + 0.5) / SQRT_SAMPLES) / data->server->height;
                    ray = generate_ray(data->server->world->cameras->content, u, v);
                    sample_color = raytracer(&ray, data->server->world, MAX_DEPTH);
                    pixel_colors[k] = sample_color;
                    k++;
                }
            }
            averaged_color = average_color(pixel_colors, SAMPLES_PER_PIXEL);
            my_put_pixel(data->server, i, data->server->height - 1 - j, averaged_color);
            i++;
        }
        j++;
    }
    pthread_exit(NULL);
} */

// Función para renderizar la escena, es similar a la función render de arriba, pero
// el único cambio es que dividimos la escena por secciones. Cada seccion será
// renderizada por un hilo, consiguiendo agilizar la computación, lo que proporciona
// una mayor fluidez.
void render(t_server *server)
{
    pthread_t       threads[NUM_THREADS];
    t_thread_data   thread_data[NUM_THREADS];
    int             rows_per_thread;
    int             rc;
    int             t;
    
    rows_per_thread= server->height / NUM_THREADS;
    if (!server->world->cameras)
        return;
    t = -1;
    while (++t < NUM_THREADS)
    {
        thread_data[t].server = server;
        thread_data[t].start_row = t * rows_per_thread;
        if (t == NUM_THREADS - 1)
            thread_data[t].end_row = server->height;
        else
            thread_data[t].end_row = thread_data[t].start_row + rows_per_thread;
        if (server->world->material)
            rc = pthread_create(&threads[t], NULL, render_section_super, (void *)&thread_data[t]);
        else
            rc = pthread_create(&threads[t], NULL, render_section, (void *)&thread_data[t]);
        check_pthread_create(rc, server);
    }
    join_menu_image(threads, server);
}

// Función para que el renderizado sea mucho más pixelado y por tanto involucre 
// menos computación (idea de Chema). Básicamente en lugar de pintar 1 pixel e iterar
// por todos los pixeles de la pantalla, pintamos un cuadrado de pixeles del mismo
// color. Esta función sólo se usa cuando nos estamos moviendo por la pantalla, nos 
// interesa verlo todo fluido y cuando tengoamos la posición deseada pulsamos el 
// espacio, para  renderizar toda la escena.
void render_low(t_server *server)
{
    int     i;
    int     j;
	int		scale_factor;
    t_ray   ray;
    t_color pixel_color;

    scale_factor = adjust_scale_factor(server);
    if (!server->world->cameras || scale_factor <= 0)
        return;
    j = -1;
    while (++j < server->height / scale_factor)
    {
        i = -1;
        while (++i < server->width / scale_factor)
        {
            ray = generate_ray(server->world->cameras->content, (float)i / (server->width / scale_factor), (float)j / (server->height / scale_factor));
            pixel_color = raytracer(&ray, server->world, MAX_DEPTH);
            fill_pixels(server, scale_factor, i, j, pixel_color);
        }
    }
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}
