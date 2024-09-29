#include <minirt.h>

void    apply_texture(t_ray *ray, t_world *world)
{
    float   u;
    float   v;
    
    if (world->texture)
    {
        u = 0;
        v = 0;
        if (ray->record.type == SPHERE)
            apply_texture_sphere(ray, world, &u, &v);
        else if (ray->record.type == SQUARE)
            apply_texture_square(ray, world, &u, &v);
        else if (ray->record.type == PLANE)
            apply_texture_plane(ray, world, &u, &v);
        else if (ray->record.type == CYLINDER)
            apply_texture_cylinder(ray, world, &u, &v);
        if (world->bump)
            ray->record.normal = apply_bump(ray->record, world, u, v);
    }
}

void    apply_material(t_ray *ray, t_world *world, int depth)
{
    if (world->material)
        treat_material(ray, world, depth);
}

void    apply_checkerboard(t_ray *ray, t_world *world)
{
    if (world->checkerboard)
        ray->record.color = checkerboard_pattern_selector(ray);
}

int check_depth_intersec(t_ray *ray, t_world *world, int depth)
{
    if (depth <= 0)
        return (0);
    if (!intersec(ray, world->figures))
        return (0);
    return (1);
}

// El raytracing se encarga de determinar la intersección del rayo
// con los objetos en la escena y calcular el color resultante basado
// en las propiedades del material, las luces, y otras consideraciones.
// Si no hay intersección con ningun objeto devuelvo color negro
t_color raytracer(t_ray *ray, t_world *world, int depth)
{
    int     vis;
    t_color color;
    t_color ambient;
    t_list  *light;
    t_light current_light;

    if (!check_depth_intersec(ray, world, depth))
        return (0x0);
    apply_texture(ray, world);
    apply_material(ray, world, depth);
    apply_checkerboard(ray, world);
    light = world->lights;
    ambient = cscale((*world->ambient).color, (*world->ambient).brightness);
    color = cproduct(ray->record.color, ambient);
    while(light)
    {
        current_light = *((t_light *)light->content);
        vis = !in_shadow(current_light, world->figures, ray->record);
        if (world->phong)
            color = cadd(color, phong_lighting(current_light, ray));
        else
            color = cadd(color, vis * color_component(current_light, ray->record));
        light = light->next;
    }
    return (color);
}