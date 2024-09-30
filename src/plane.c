#include <minirt.h>

t_plane	*new_plane(char **data)
{
	t_plane	*plane;

	if (double_pointer_len(data) != 4)
		return (NULL);
	plane = malloc(sizeof(*plane));
	if (!plane)
		return (NULL);
	plane->type = PLANE;
	plane->point = ft_atov(data[1]);
	plane->normal = norm(ft_atov(data[2]));
	plane->color = ft_atoc(data[3]);
	plane->material.type = 1;
	return (plane);
}

int	hit_plane(t_ray *ray, t_plane *plane)
{
	double	t;
	double	den;

	den = dot(norm(ray->direction), plane->normal);
	if (!den)
		return (0);
	t = dot(sub(plane->point, ray->origin), plane->normal) / den;
	if (ray->record.t > t && t > EPSILON)
	{
		if (dot(ray->direction, plane->normal) > 0)
			plane->normal = scale(plane->normal, -1);
		ray->record.t = t;
		ray->record.p = at(*ray);
		ray->record.normal = plane->normal;
		ray->record.color = plane->color;
		ray->record.type = plane->type;
		ray->record.object = plane;
		if (plane->material.type)
			fill_glass_material(ray);
		else
			fill_mate_material(ray);
		return (1);
	}
	return (0);
}
