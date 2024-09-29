#include <minirt.h>

t_color reflect(t_ray *ray, t_world *world, int depth)
{
    t_vector reflected_v;
    t_ray reflected_ray;
    
	if (depth <= 0)
        return (0x0);
	reflected_v = reflect_vector(ray->direction, ray->record.normal);
    reflected_ray.origin = ray->record.p,
    reflected_ray.direction = norm(reflected_v);
    return (raytracer(&reflected_ray, world, depth - 1));
}

t_color refract(t_ray *ray, t_world *world, int depth)
{
    t_ray		refracted_ray;
    t_vector	refracted_vector;
    float		env_ior;
    
	if (depth <= 0)
        return (0x0);
	env_ior = 4.0 * ALBEDO;
	refracted_vector = refract_vector(ray->direction, ray->record.normal, ray->record.material.ior, env_ior);
    if (length(refracted_vector) == 0)
        return (0x0);
    refracted_ray.origin = ray->record.p,
    refracted_ray.direction = norm(refracted_vector);
    return (raytracer(&refracted_ray, world, depth - 1));
}