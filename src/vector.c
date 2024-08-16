#include <minirt.h>

// Función que crea y rellena un vector dadas tres coordenadas
t_vector	vector(double x, double y, double z)
{
	t_vector	v;

	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

// Función que crea y rellena un punto dadas tres coordenadas
t_point	point(double x, double y, double z)
{
	return ((t_point)vector(x, y, z));
}
