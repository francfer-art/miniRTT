#include <minirt.h>

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