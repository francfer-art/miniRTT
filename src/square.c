#include <minirt.h>

void	square_vertex(t_square *square)
{
	t_vector	basis_i;
	t_vector	basis_j;

	basis_i = cross(square->normal, vector(square->normal.z, square->normal.x,
				square->normal.y));
	basis_j = cross(square->normal, basis_i);
	basis_i = scale(basis_i, square->side / 2);
	basis_j = scale(basis_j, square->side / 2);
	square->vertex[0] = add(add(square->center, basis_i), basis_j);
	square->vertex[1] = sub(add(square->center, basis_i), basis_j);
	square->vertex[2] = sub(sub(square->center, basis_i), basis_j);
	square->vertex[3] = add(sub(square->center, basis_i), basis_j);
}

t_square	*new_square(char **data)
{
	t_square	*square;

	if (double_pointer_len(data) != 5)
		return (NULL);
	square = malloc(sizeof(*square));
	if (!square)
		return (NULL);
	square->type = SQUARE;
	square->center = ft_atov(data[1]);
	square->normal = norm(ft_atov(data[2]));
	square->side = ft_atof(data[3]);
	square->color = ft_atoc(data[4]);
	square_vertex(square);
	square->material.type = 1;
	return (square);
}

int	is_inside(t_hit r, t_vector *v, int vertex)
{
	int			i;
	int			in;
	float		det[4];
	t_vector	det_cross;

	in = 1;
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
			if ((det[i - 1] >= 0 && det[i] < 0) || (det[i - 1] < 0
					&& det[i] >= 0))
				in = 0;
		}
		i++;
	}
	return (in);
}

int	hit_square(t_ray *ray, t_square *square)
{
	int		hit;
	t_ray	r;
	t_plane	plane;

	hit = 0;

    memset(&r, 0, sizeof(t_ray));
    memset(&plane, 0, sizeof(t_plane));
	plane.point = square->center;
	plane.normal = square->normal;
	plane.color = square->color;
	r.origin = ray->origin;
	r.direction = ray->direction;
	r.record.t = ray->record.t;
	if (hit_plane(&r, &plane) && is_inside(r.record, square->vertex, 4))
	{
		*ray = r;
		ray->record.type = SQUARE;
		ray->record.object = square;
		if (square->material.type)
			fill_glass_material(ray);
		else
			fill_mate_material(ray);
		hit = 1;
	}
	return (hit);
}
