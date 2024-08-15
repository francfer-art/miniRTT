#include <minirt.h>

int intersec(t_ray *ray, t_list *figures)//Devolverá 1 si hay intersección con algún objeto y 0 si no la hay
{

}

t_color raytracer(t_ray *ray, t_world *world)
{
    int     vis;
    t_color color;
    t_color ambient;
    t_list  *light;
    t_light current_light;

    if (!intersec(ray, world->figures))//Si no hay intersección con ningun objeto devuelvo color negro
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