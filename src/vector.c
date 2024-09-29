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

//Función que "escala" un vector, multiplica todas sus componentes por un escalar
t_vector	scale(t_vector v, double c)
{
	return (vector(v.x * c, v.y * c, v.z * c));
}

// Función que eleva al cuadrado un número
double	sqr(double num)
{
	return (num * num);
}

// Función que dado un vector, eleva al cuadrado cada componente del vector y suma 
// sus valores
double	length_square(t_vector v)
{
	return (sqr(v.x) + sqr(v.y) + sqr(v.z));
}

// Función que dado un vector, realiza la raíz cuadrada de la suma de los
// cuadrados de cada componente
double	length(t_vector v)
{
	return (sqrt(length_square(v)));
}