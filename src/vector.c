#include <minirt.h>

t_vector	vector(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

t_point	point(double x, double y, double z)
{
	return ((t_point)vector(x, y, z));
}
