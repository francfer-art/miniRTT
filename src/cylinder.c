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

void	cylinder_roots(t_ray r, t_cylinder cylinder, float roots[4])
{
	t_vector	v;
	t_vector	w;
	float		f[4];

	v = sub(r.direction, scale(cylinder.axis, dot(r.direction, cylinder.axis)));
	w = sub(sub(r.origin, cylinder.center), scale(cylinder.axis,
				dot(sub(r.origin, cylinder.center), cylinder.axis)));
	f[0] = length_square(v);
	f[1] = dot(v, w);
	f[2] = length_square(w) - pow(cylinder.radius, 2);
	f[3] = pow(f[1], 2) - f[0] * f[2];
	if (f[3] < 0)
	{
		roots[2] = INFINITY;
		roots[3] = INFINITY;
		return ;
	}
	roots[2] = (-f[1] - sqrt(f[3])) / f[0];
	roots[3] = (-f[1] + sqrt(f[3])) / f[0];
}

void	valid_hit(int *hit, float w[4], float *d, float *t)
{
	if (hit[0] && hit[1])
	{
		if (w[2] < w[3])
		{
			*d = w[0];
			*t = w[2];
		}
		else
		{
			*d = w[1];
			*t = w[3];
		}
	}
	else if (hit[0])
	{
		*d = w[0];
		*t = w[2];
	}
	else if (hit[1])
	{
		*d = w[1];
		*t = w[3];
	}
}

float	solve(t_ray r, t_cylinder cy, float *d, int hit[2])
{
	float		t;
	t_vector	b_ray;
	float		w[4];

	cylinder_roots(r, cy, w);
	b_ray = sub(r.origin, cy.center);
	w[0] = dot(cy.axis, sub(add(r.origin, scale(r.direction, w[2])),
				cy.center));
	w[1] = dot(cy.axis, sub(add(r.origin, scale(r.direction, w[3])),
				cy.center));
	hit[0] = (w[0] >= 0 && w[0] <= cy.height && w[2] > EPSILON);
	hit[1] = (w[1] >= 0 && w[1] <= cy.height && w[3] > EPSILON);
	valid_hit(hit, w, d, &t);
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
