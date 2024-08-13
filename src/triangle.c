#include <minirt.h>

t_triangle	*new_triangle(char **data)
{
	t_triangle	*triangle;
	t_vector	*v;

	if (double_pointer_len(data) != 5)
		return (NULL);
	triangle = malloc(sizeof(*triangle));
	if (!triangle)
		return (NULL);
	v = triangle->vertex;
	triangle->type = TRIANGLE;
	triangle->color = ft_atoc(data[4]);
	triangle->vertex[0] = ft_atov(data[3]);
	triangle->vertex[1] = ft_atov(data[2]);
	triangle->vertex[2] = ft_atov(data[1]);
	triangle->normal = norm(cross(sub(v[1], v[0]), sub(v[2], v[0])));
	return (triangle);
}

static int	is_inside(t_hit	r, t_vector *v, int vertex)
{
	int			i;
	int			in;
	float		det[vertex];
	t_vector	det_cross;

	i = 0;
	while (i < vertex)
	{
		if (i == vertex - 1)
			det_cross = cross(sub(v[i], v[0]), sub(r.p, v[0]));
		else
			det_cross = cross(sub(v[i], v[i + 1]), sub(r.p, v[i + 1]));
		det[i] = dot(det_cross, r.normal);
		if (i > 0)
		{
			if ((det[i - 1] >= 0 && det[i] < 0) || (det[i - 1] < 0 && det[i] >= 0))
				in = true;
			else
				in = false;
		}
		i++;
	}
}

int	hit_triangle(t_ray *ray, t_triangle *triangle)
{
	int		hit;
	t_ray	r;
	t_plane	pl;

	hit = false;
	pl.point = triangle->vertex[0];
	pl.normal = triangle->normal;
	pl.color = triangle->color;
	r.direction = ray->direction;
	r.origin = ray->origin;
	r.record.t = ray->record.t;
	if (hit_plane(&r, &pl) && is_inside(r.record, triangle->vertex, 3))
	{
		*ray = r;
		ray->record.type = TRIANGLE;
		hit = true;
	}
	return (hit);
}
