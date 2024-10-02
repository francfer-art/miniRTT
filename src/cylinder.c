#include <minirt.h>

t_cylinder	*new_cylinder(char **data)
{
	t_cylinder	*cylinder;

	if (double_pointer_len(data) != 6)
		return (NULL);
	cylinder = malloc(sizeof(*cylinder));
	if (!cylinder)
		return (NULL);
	cylinder->type = CYLINDER;
	cylinder->color = ft_atoc(data[5]);
	cylinder->height = ft_atof(data[4]);
	cylinder->radius = ft_atof(data[3]) / 2;
	cylinder->axis = norm(ft_atov(data[2]));
	cylinder->center = ft_atov(data[1]);
	return (cylinder);
}

void	cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2])
{
	t_vector	v;
	t_vector	w;
	float		a;
	float		half_b;
	float		c;
	float		discriminant;

	v = sub(r.direction, scale(cylinder.axis, dot(r.direction, cylinder.axis)));
	w = sub(sub(r.origin, cylinder.center), scale(cylinder.axis,
				dot(sub(r.origin, cylinder.center), cylinder.axis)));
	a = length_square(v);
	half_b = dot(v, w);
	c = length_square(w) - pow(cylinder.radius, 2);
	discriminant = pow(half_b, 2) - a * c;
	if (discriminant < 0)
	{
		roots[0] = INFINITY;
		roots[1] = INFINITY;
		return ;
	}
	roots[0] = (-half_b - sqrt(discriminant)) / a;
	roots[1] = (-half_b + sqrt(discriminant)) / a;
}

void	valid_hit(int *hit, float *dist, float *root, float *d, float *t)
{
	if (hit[0] && hit[1])
	{
		if (root[0] < root[1])
		{
			*d = dist[0];
			*t = root[0];
		}
		else
		{
			*d = dist[1];
			*t = root[1];
		}
	}
	else if (hit[0])
	{
		*d = dist[0];
		*t = root[0];
	}
	else if (hit[1])
	{
		*d = dist[1];
		*t = root[1];
	}
}

float	solve(t_ray r, t_cylinder cy, float *d, int hit[2])
{
	float		t;
	float		dist[2];
	float		root[2];
	t_vector	b_ray;

	cylinder_roots(r, cy, root);
	b_ray = sub(r.origin, cy.center);
	dist[0] = dot(cy.axis, sub(add(r.origin, scale(r.direction, root[0])),
				cy.center));
	dist[1] = dot(cy.axis, sub(add(r.origin, scale(r.direction, root[1])),
				cy.center));
	hit[0] = (dist[0] >= 0 && dist[0] <= cy.height && root[0] > EPSILON);
	hit[1] = (dist[1] >= 0 && dist[1] <= cy.height && root[1] > EPSILON);
	valid_hit(hit, dist, root, d, &t);
	return (t);
}

int	hit_cylinder(t_ray *ray, t_cylinder *cylinder)
{
	float		y;
	float		t;
	int			hit[2];
	t_vector	point_on_axis;

	t = solve(*ray, *cylinder, &y, hit);
	if ((hit[0] || hit[1]) && ray->record.t > t && t > EPSILON)
	{
		ray->record.t = t;
		ray->record.p = at(*ray);
		point_on_axis = add(cylinder->center, scale(cylinder->axis, y));
		ray->record.normal = norm(sub(ray->record.p, point_on_axis));
		if (hit[1] && !hit[0])
			ray->record.normal = scale(ray->record.normal, -1);
		ray->record.color = cylinder->color;
		return (1);
	}
	return (0);
}
