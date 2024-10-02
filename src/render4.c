#include <minirt.h>

void	apply_texture_cone(t_ray *ray, t_world *world, float *u, float *v)
{
	t_cone		*cone;
	t_vector	p_minus_vertex;
	t_vector	around_axis;
	float		height_along_axis;
	float		theta;

	cone = ray->record.object;
	p_minus_vertex = sub(ray->record.p, cone->vertex);
	height_along_axis = dot(p_minus_vertex, norm(cone->direction));
	if (height_along_axis < 0)
		height_along_axis = 0;
	if (height_along_axis > cone->height)
		height_along_axis = cone->height;
	*v = height_along_axis / cone->height;
	around_axis = sub(p_minus_vertex, scale(norm(cone->direction), height_along_axis));
	theta = atan2(around_axis.z, around_axis.x);
	*u = 1 - (theta / (2 * M_PI));
	ray->record.color = get_texture_color(world->texture_img, *u, *v);
}


void	apply_texture(t_ray *ray, t_world *world)
{
	float	u;
	float	v;

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
		else if (ray->record.type == CONE)
			apply_texture_cone(ray, world, &u, &v);
		if (world->bump)
			ray->record.normal = apply_bump(ray->record, world, u, v);
	}
}

void	apply_material(t_ray *ray, t_world *world, int depth)
{
	if (world->material)
		treat_material(ray, world, depth);
}

void	apply_checkerboard(t_ray *ray, t_world *world)
{
	if (world->checkerboard)
		ray->record.color = checkerboard_pattern_selector(ray);
}

int	check_depth_intersec(t_ray *ray, t_world *world, int depth)
{
	if (depth <= 0)
		return (0);
	if (!intersec(ray, world->figures))
		return (0);
	return (1);
}

t_color	raytracer(t_ray *ray, t_world *world, int depth)
{
	int		vis;
	t_color	color;
	t_color	ambient;
	t_list	*light;
	t_light	current_light;

	if (!check_depth_intersec(ray, world, depth))
		return (0x0);
	apply_stuff(ray, world, depth);
	light = world->lights;
	ambient = cscale((*world->ambient).color, (*world->ambient).brightness);
	color = cproduct(ray->record.color, ambient);
	while (light)
	{
		current_light = *((t_light *)light->content);
		vis = !in_shadow(current_light, world->figures, ray->record);
		if (world->phong)
			color = cadd(color, phong_lighting(current_light, ray));
		else
			color = cadd(color, vis * color_component(current_light,
						ray->record));
		light = light->next;
	}
	return (color);
}
