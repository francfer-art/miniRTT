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
	cylinder->axis = ft_atov(data[2]);
	cylinder->center = ft_atov(data[1]);
	if (out_range_vector(cylinder->axis))
		message_exit(ERROR_VECTOR);
	else
		cylinder->axis = norm(cylinder->axis);
}

void	cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2])
{
	float		a;
	float		half_b;
	float		c;
	t_vector	v;
	t_vector	w;

	v = scale(cylinder.axis, dot(r.direction, cylinder.axis));
	v = sub(r.direction, v);
	w = scale(cylinder.axis, (dot(sub(r.origin, cylinder.center),
					cylinder.axis)));
	w = sub(sub(r.origin, cylinder.center), v);
	a = length_square(v);
	half_b = dot(v, w);
	c = length_square(w) - pow(cylinder.radius, 2);
	roots[0] = (-half_b + sqrt(pow(half_b, 2) - a * c)) / a;
	roots[1] = (-half_b - sqrt(pow(half_b, 2) - a * c)) / a;
}

void	valid_hit(int *hit, float *dist, float *root, float *d, float *t)
{
	if (hit[0] && hit[1])
	{
		if (root[0] < root[1])
		{
			*d = dist[0];
			*t = root[0];
		}else
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
	else if (hit[hit[1]])
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
	b_ray = sub(cy.center, r.origin);
	dist[0] = dot(cy.axis, sub(scale(r.direction, root[0]), b_ray));
	dist[1] = dot(cy.axis, sub(scale(r.direction, root[1]), b_ray));
	hit[0] = (dist[0] >= 0 && dist[0] <= cy.height && root[0] > EPSILON);
	hit[1] = (dist[1] >= 0 && dist[1] <= cy.height && root[1] > EPSILON);
	valid_hit(hit, dist, root, d, &t);
	return (t);
}

int	hit_cylinder(t_ray *ray, t_cylinder *cylinder)
{
	float	y;
	float	t;
	int		hit[2];

	t = solve(*ray, *cylinder, &y, hit);
	if ((hit[0] || hit[1]) && ray->record.t > t && t > EPSILON)
	{
		ray->record.t = t;
		ray->record.p = at(*ray);
		ray->record.normal = norm(sub(ray->record.p, add(scale(cylinder->axis, y), cylinder->center)));
		if (hit[1] && !hit[0])
			ray->record.normal = scale(ray->record.normal, -1);
		ray->record.color = cylinder->color;
	}
	return (hit[0] || hit[1]);
}
