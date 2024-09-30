#include <minirt.h>

t_color	checkerboard_square(t_ray *ray)
{
	int			check_x;
	int			check_y;
	int			checker;
	float		size;
	t_square	*square;

	square = ray->record.object;
	size = square->side;
	check_x = (int)floor(ray->record.p.z / size);
	check_y = (int)floor(ray->record.p.x / size);
	checker = (check_x + check_y) % 2;
	if (checker == 0)
		return (ray->record.color);
	else
		return (0xFFFFFF - ray->record.color);
}

t_color	checkerboard_sphere(t_ray *ray, float size)
{
	int	check_x;
	int	check_y;
	int	check_z;
	int	checker;

	check_x = (int)floor(ray->record.p.x / size);
	check_y = (int)floor(ray->record.p.y / size);
	check_z = (int)floor(ray->record.p.z / size);
	checker = (check_x + check_y + check_z) % 2;
	if (checker == 0)
		return (ray->record.color);
	else
		return (0xFFFFFF - ray->record.color);
}

t_color	checkerboard_cylinder(t_ray *ray)
{
	t_cylinder	*cylinder;
	float		u;
	float		v;
	float		theta;

	cylinder = ray->record.object;
	theta = atan2(ray->record.p.z / cylinder->radius, ray->record.p.x
			/ cylinder->radius);
	u = (theta + M_PI) / (2 * M_PI);
	v = ray->record.p.y / cylinder->radius;
	if (v <= 0)
		v += 1.0;
	if (!((((int)floor(u * cylinder->radius) + (int)floor(v * cylinder->radius))
				% 2)))
		return (ray->record.color);
	else
		return (0xFFFFFF - ray->record.color);
}

t_color	checkerboard_pattern_selector(t_ray *ray)
{
	if (ray->record.type == SPHERE)
		return (checkerboard_sphere(ray, 1.0));
	else if (ray->record.type == PLANE)
		return (checkerboard_sphere(ray, 3.0));
	else if (ray->record.type == SQUARE)
		return (checkerboard_square(ray));
	else if (ray->record.type == CYLINDER)
		return (checkerboard_cylinder(ray));
	else
		return (checkerboard_sphere(ray, 1.0));
	return (ray->record.color);
}

void	treat_material(t_ray *ray, t_world *world, int depth)
{
	t_color	reflected_color;
	t_color	refracted_color;

	if (ray->record.type == SPHERE || ray->record.type == PLANE
		|| ray->record.type == SQUARE)
	{
		reflected_color = reflect(ray, world, depth);
		ray->record.color = cadd(ray->record.color, cscale(reflected_color,
					ray->record.material.reflectivity));
	}
	if (ray->record.type == SPHERE || ray->record.type == PLANE
		|| ray->record.type == SQUARE)
	{
		refracted_color = refract(ray, world, depth);
		ray->record.color = cadd(ray->record.color, cscale(refracted_color,
					ray->record.material.refractivity));
	}
}
