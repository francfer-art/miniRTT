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

// El raytracing se encarga de determinar la intersección del rayo
// con los objetos en la escena y calcular el color resultante basado
// en las propiedades del material, las luces, y otras consideraciones.
// Si no hay intersección con ningun objeto devuelvo color negro
t_color raytracer(t_ray *ray, t_world *world)
{
    int     vis;
    t_color color;
    t_color ambient;
    t_list  *light;
    t_light current_light;

    if (!intersec(ray, world->figures))
        return (0x0);
    light = world->lights;
    ambient = cscale((*world->ambient).color, (*world->ambient).brightness);
    color = cproduct(ray->record.color, ambient);
    while(light)
    {
        current_light = *((t_light *)light->content);
        vis = !in_shadow(current_light, world->figures, ray->record);
        color = cadd(color, vis * color_component(current_light, ray->record));
        light = light->next;
    }
    return (color);
}

// La función render se encargará de renderizar la escena y mostrarla en una 
// ventana gráfica
// Si no hay cámaras, salimos del renderizado
// Hacemos un bucle anidado, idéntico al de fractol. Recorrerá todos los pixeles 
// de pantalla e irá :
//      Generando el rayo, calculando el color con la función raytracer() y poniendo en 
//      la imagen el pixel con el nuevo color
// Tras salir del bucle pondremos la imagen por pantalla mlx_put_image_to_window
// void    render(t_server *server)
// {
//     int     i;
//     int     j;
//     t_ray   ray;
//     t_color pixel_color;

//     if (!server->world->cameras)
//         return ;
//     j = server->height;
//     while (j-- > 0)
//     {
//         i = server->width;
//         while (i-- > 0)
//         {
//             ray = generate_ray(server->world->cameras->content, (float)i / server->width, (float)j / server->height);
//             pixel_color = raytracer(&ray, server->world);
//             my_put_pixel(server, i, server->height - 1 - j, pixel_color);
//         }
//     }
//     mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
// }

// Función que realiza el render en una sección delimitada. Dicha sección la realiza
// un hilo diferente. La idea es dividir la ventana en tantos cuadrados como hilos
// tengae el procesador
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
            pixel_color = raytracer(&ray, data->server->world);
            my_put_pixel(data->server, i, data->server->height - 1 - j, pixel_color);
            i++;
        }
    j++;
    }
    pthread_exit(NULL);
}

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
        rc = pthread_create(&threads[t], NULL, render_section, (void *)&thread_data[t]);
        if (rc)
            full_message_exit(ERROR_CREATE, NULL, server);
    }
    t = -1;
    while (++t < NUM_THREADS)
        if (pthread_join(threads[t], NULL))
            full_message_exit(ERROR_JOIN, NULL, server);
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}

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

// Función para que el renderizado sea mucho más pixelado y por tanto involucre 
// menos computación (idea de Chema). Básicamente en lugar de pintar 1 pixel e iterar
// por todos los pixeles de la pantalla, pintamos un cuadrado de pixeles del mismo
// color. Esta función sólo se usa cuando nos estamos moviendo por la pantalla, nos 
// interesa verlo todo fluido y cuando tengoamos la posición deseada pulsamos el 
// espacio, para  renderizar toda la escena.
void render_low(t_server *server, int scale_factor)
{
    int     i;
    int     j;
    t_ray   ray;
    t_color pixel_color;

    if (!server->world->cameras || scale_factor <= 0)
        return;
    int low_width = server->width / scale_factor;
    int low_height = server->height / scale_factor;
    j = -1;
    while (++j < server->height / scale_factor)
    {
        i = -1;
        while (++i < server->width / scale_factor)
        {
            ray = generate_ray(server->world->cameras->content, (float)i / low_width, (float)j / low_height);
            pixel_color = raytracer(&ray, server->world);
            fill_pixels(server, scale_factor, i, j, pixel_color);
        }
    }
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}
