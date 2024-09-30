#include <minirt.h>

double	dot(t_vector v, t_vector w)
{
	return ((v.x * w.x) + (v.y * w.y) + (v.z * w.z));
}

t_vector	add(t_vector v, t_vector w)
{
	return (vector(v.x + w.x, v.y + w.y, v.z + w.z));
}

t_vector	sub(t_vector v, t_vector w)
{
	return (vector(v.x - w.x, v.y - w.y, v.z - w.z));
}

t_vector	at(t_ray ray)
{
	return (add(ray.origin, scale(ray.direction, ray.record.t)));
}

t_vector	cross(t_vector v, t_vector w)
{
	t_vector	u;

	u.x = (v.y * w.z) - (v.z * w.y);
	u.y = (v.z * w.x) - (v.x * w.z);
	u.z = (v.x * w.y) - (v.y * w.x);
	return (u);
}
