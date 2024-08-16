#include <minirt.h>

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

// Función que realiza el producto punto de dos vectores
// El resultado será la suma de la multiplicación de cada componente de cada
// vector
double	dot(t_vector v, t_vector w)
{
	return ((v.x * w.x) + (v.y * w.y) + (v.z * w.z));
}

// Función que suma dos vectores, devolverá el vector resultante de la suma de 
// cada componente de cada vector. 
// Vector1 = (a,b,c) Vector2=(d,e,f)
// Vector3 = Vector1 + Vector2 = (a+d, b+e, c+f)
t_vector	add(t_vector v, t_vector w)
{
	return (vector(v.x + w.x, v.y + w.y, v.z + w.z));
}

// Función que resta dos vectores, devolverá el vector resultante de la resta de 
// cada componente de cada vector. 
// Vector1 = (a,b,c) Vector2=(d,e,f)
// Vector3 = Vector1 - Vector2 = (a-d, b-e, c-f)
t_vector	sub(t_vector v, t_vector w)
{
	return (vector(v.x - w.x, v.y - w.y, v.z - w.z));
}

// Función que calcula un punto en el espacio tridimennsional a lo largo de un rayo
// Escalamos el vector de dirección del rayo por el parámetro t, de este modo,
// conseguimos saber que tan lejos está el punto 
// A la posición calculada le sumamos la posción del origen del rayo
t_vector	at(t_ray ray)
{
	return (add(ray.origin, scale(ray.direction, ray.record.t)));
}

// Función para calcular el prodcuto vectorial de dos vectores, en resumen, calcula
// el vector que es perpendicular a ambos vectores
// El resultado se obtiene mediante determinantes y calculos matriciales
t_vector	cross(t_vector v, t_vector w)
{
	t_vector	u;

	u.x = (v.y * w.z) - (v.z * w.y);
	u.y = (v.z * w.x) - (v.x * w.z);
	u.z = (v.x * w.y) - (v.y * w.x);
	return (u);
}
