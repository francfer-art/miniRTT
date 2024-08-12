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

int	is_inside()
{
	
}