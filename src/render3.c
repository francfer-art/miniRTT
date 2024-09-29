#include <minirt.h>

t_vector apply_bump(t_hit record, t_world *world, float u, float v)
{
    t_vector    perturbed_normal;
    t_vector    bump;
    
    perturbed_normal = record.normal;
    bump = bump_function(record, world->texture_img, u, v);
    perturbed_normal = add(perturbed_normal, bump);
    return (norm(perturbed_normal));
}

void    apply_texture_sphere(t_ray *ray, t_world *world, float *u, float *v)
{
    *u = 1 - (0.5 + (atan2(ray->record.normal.z, ray->record.normal.x) / (2 * M_PI)));
    *v = 0.5 - (asin(ray->record.normal.y) / M_PI);
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_square(t_ray *ray, t_world *world, float *u, float *v)
{
    t_square    *square;
    t_vector    u_axis;
    t_vector    v_axis;
    t_vector    relative_p;

    square = ray->record.object;
    if (fabs(square->normal.y) < 0.999)
        u_axis = (cross(square->normal, vector(0, 1, 0)));
    else
        u_axis = (cross(square->normal, vector(1, 0, 0)));
    v_axis = (cross(square->normal, u_axis));
    relative_p = sub(ray->record.p, square->center);
    *u = 1 - dot(relative_p, u_axis) / square->side + 0.5;
    *v = dot(relative_p, v_axis) / square->side + 0.5;
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_plane(t_ray *ray, t_world *world, float *u, float *v)
{
    t_plane     *plane;
    t_vector    u_axis;
    t_vector    v_axis;
    t_vector    relative_p;
    float       texture_scale;

    plane = ray->record.object;
    if (fabs(plane->normal.y) < 0.999)
        u_axis = cross(plane->normal, vector(0, 1, 0));
    else
        u_axis = cross(plane->normal, vector(1, 0, 0));
    v_axis = cross(plane->normal, u_axis);
    relative_p = sub(ray->record.p, plane->point);
    *u = dot(relative_p, u_axis);
    *v = dot(relative_p, v_axis);
    texture_scale = 100.0;
    *u = (*u / texture_scale) - floor(*u / texture_scale);
    *v = (*v / texture_scale) - floor(*v / texture_scale);
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_cylinder(t_ray *ray, t_world *world, float *u, float *v)
{
    t_cylinder  *cylinder;
    t_vector    p_minus_base;
    t_vector    around_axis;
    float       height_along_axis;
    float       theta;

    cylinder = ray->record.object;
    p_minus_base = sub(ray->record.p, cylinder->center);
    height_along_axis = dot(p_minus_base, norm(cylinder->axis));
    if (height_along_axis < 0)
        height_along_axis = 0;
    if (height_along_axis > cylinder->height)
        height_along_axis = cylinder->height;
    *v = height_along_axis / cylinder->height;
    around_axis= sub(p_minus_base, scale(norm(cylinder->axis), height_along_axis));
    theta = atan2(around_axis.z, around_axis.x);
    *u = 1 - (theta / (2 * M_PI));
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}