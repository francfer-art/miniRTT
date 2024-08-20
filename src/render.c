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
//Si no hay intersección con ningun objeto devuelvo color negro
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
    ambient = cscale();
    color = cproduct();
    while(light)
    {
        current_light = *((t_light *)light->content);
        vis = !in_shadow();
        color = cadd();
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
void    render(t_server *server)
{
    int     i;
    int     j;
    t_ray   ray;
    t_color pixel_color;

    if (!server->world->cameras)
        return ;
    j = server->height;
    while (j > 0)
    {
        i = server->width;
        while (i > 0)
        {
            ray = generate_ray(server->world->cameras->content, (float)i / server->width, (float)j / server->height);
            pixel_color = raytracer(&ray, server->world);
            my_put_pixel(server, i, server->height - 1 - j, pixel_color);
            i--;
        }
        j--;
    }
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}