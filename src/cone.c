#include <minirt.h>

void	solve_quadratic(float a, float b, float c, float *roots,
		int *root_count)
{
	float	discriminant;

	discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		*root_count = 2;
		roots[0] = (-b + sqrt(discriminant)) / (2 * a);
		roots[1] = (-b - sqrt(discriminant)) / (2 * a);
	}
	else if (discriminant == 0)
	{
		*root_count = 1;
		roots[0] = -b / (2 * a);
	}
	else
	{
		*root_count = 0;
	}
}

void	cone_roots(t_ray ray, t_cone cone, float *roots, int *root_count)
{
	t_vector	oc;
	float		a;
	float		b;
	float		c;
	float		cos_theta_sq;

	oc = sub(ray.origin, cone.vertex);
	cos_theta_sq = cos(cone.angle) * cos(cone.angle);
	a = dot(ray.direction, cone.direction) * dot(ray.direction, cone.direction)
		- cos_theta_sq * dot(ray.direction, ray.direction);
	b = 2 * (dot(ray.direction, cone.direction) * dot(oc, cone.direction)
			- cos_theta_sq * dot(ray.direction, oc));
	c = dot(oc, cone.direction) * dot(oc, cone.direction) - cos_theta_sq
		* dot(oc, oc);
	solve_quadratic(a, b, c, roots, root_count);
}

t_vector	cone_normal(t_cone cone, t_vector point)
{
	t_vector	v;
	t_vector	axis_component;
	t_vector	normal;

	v = sub(point, cone.vertex);
	axis_component = scale(cone.direction, dot(v, cone.direction));
	normal = sub(v, scale(axis_component, cos(cone.angle) * cos(cone.angle)));
	return (norm(normal));
}

int	hit_cone(t_ray *ray, t_cone *cone)
{
	unsigned int	i;
	float			roots[2];
	int				root_count;
	t_vector		point;
	float			dist_to_vertex;

	cone_roots(*ray, *cone, roots, &root_count);
	for (i = 0; i < root_count; i++)
	{
		if (ray->record.t > roots[i] && roots[i] > EPSILON)
		{
			point = add(ray->origin, scale(ray->direction, roots[i]));
			dist_to_vertex = dot(sub(point, cone->vertex), cone->direction);
			if (fabs(dist_to_vertex) <= cone->height)
			{
				ray->record.t = roots[i];
				ray->record.p = point;
				ray->record.normal = cone_normal(*cone, ray->record.p);
				ray->record.color = cone->color;
				ray->record.type = CONE;
				return (1);
			}
		}
	}
	return (0);
}

t_cone	*new_cone(char **data)
{
	t_cone	*cone;

	if (double_pointer_len(data) != 6)
		return (NULL);
	cone = malloc(sizeof(*cone));
	if (!cone)
		return (NULL);
	cone->type = CONE;
	cone->vertex = ft_atov(data[1]);
	cone->direction = norm(ft_atov(data[2]));
	cone->angle = ft_atof(data[3]) * M_PI / 180;
	cone->height = ft_atof(data[4]);
	cone->color = ft_atoc(data[5]);
	return (cone);
}
