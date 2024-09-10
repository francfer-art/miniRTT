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

t_vector	negate(t_vector v)
{
	return (vector(-v.x, -v.y, -v.z));
}

float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

t_vector	reflect_vector(t_vector v, t_vector normal)
{
	return (sub(v, scale(normal, 2.0 * dot(v, normal))));
}

t_vector	refract_vector(t_vector v, t_vector normal, float ior, float env_ior)
{
	float		cosi;
	float		etai;
	float		etat;
	t_vector	n;

	cosi = clamp(dot(v, normal), -1.0, 1.0);
	etai = env_ior;
	etat = ior;
	n = normal;
	if (cosi < 0)
		cosi = -cosi;
	else
	{
		float temp = etai;
		etai = etat;
		etat = temp;
		n = negate(normal);
	}
	float eta = etai / etat;
	float k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0)
		return (vector(0, 0, 0));
	return (add(scale(v, eta), scale(n, (eta * cosi -sqrt(k)))));
}
// Función que calcula la reflexión de un rayo basado en la dirección del rayo y de la normal
// del objeto con el que intersecciona. Este rayo incluye la información del material del 
// obejto interseccionado
// t_color	reflect(t_ray *ray, t_world *world)
// {
// 	t_vector	reflected_v;
// 	t_ray		reflected_ray;
// 	t_color		refleted_color;

// 	reflected_v = reflect_vector(ray->direction, ray->record.normal);
// 	reflected_ray.origin = ray->record.p;
// 	reflected_ray.direction = norm(reflected_v);
// 	refleted_color = raytracer(&reflected_ray, world);
// 	return (refleted_color);
// }

// Función que calcula la refracción de un rayo basado en la ley de Snell, usando el índice
// de refracción del material y del ambiente.
// t_color	refract(t_ray *ray, t_world *world)
// {
// 	float		env_ior;
// 	t_vector	refracted_vector;
// 	t_ray		refracted_ray;
// 	t_color		refracted_color;

// 	env_ior = 1.0;
// 	refracted_vector = refract_vector(ray->direction, ray->record.normal, ray->record.material.ior, env_ior);
// 	if (length(refracted_vector) == 0)
// 		return (0x0);
// 	refracted_ray.origin = ray->record.p;
// 	refracted_ray.direction = norm(refracted_vector);
// 	refracted_color = raytracer(&refracted_ray, world);
// 	return (refracted_color);
// }

t_color reflect(t_ray *ray, t_world *world, int depth)
{
    if (depth <= 0)
        return (0x0);

    t_vector reflected_v = reflect_vector(ray->direction, ray->record.normal);
    t_ray reflected_ray = {
        .origin = ray->record.p,
        .direction = norm(reflected_v),
        .record = (t_hit){0}  // Inicializar el record para evitar problemas
    };
    return raytracer(&reflected_ray, world, depth - 1);
}

t_color refract(t_ray *ray, t_world *world, int depth)
{
    if (depth <= 0)
        return (0x0);

    float env_ior = 4.0 * ALBEDO;
    t_vector refracted_vector = refract_vector(ray->direction, ray->record.normal, ray->record.material.ior, env_ior);

    if (length(refracted_vector) == 0)
        return (0x0);

    t_ray refracted_ray = {
        .origin = ray->record.p,
        .direction = norm(refracted_vector),
        .record = (t_hit){0}  // Inicializar el record para evitar problemas
    };
    return raytracer(&refracted_ray, world, depth - 1);
}
