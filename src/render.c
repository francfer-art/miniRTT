#include <minirt.h>

// Devolverá 1 si hay alguna intersección con algún objeto de la escena,
// y devolverá 0 si no hay intersección
int intersec(t_ray *ray, t_list *figures)
{

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