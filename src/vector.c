#include <minirt.h>

t_vector	vector(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_vector	scale(t_vector v, double c)
{
	return (vector(v.x * c, v.y * c, v.z * c));
}

double	sqr(double num)
{
	return (num * num);
}

double	length_square(t_vector v)
{
	return (sqr(v.x) + sqr(v.y) + sqr(v.z));
}

double	length(t_vector v)
{
	return (sqrt(length_square(v)));
}
