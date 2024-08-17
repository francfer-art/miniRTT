#include <minirt.h>

// Función que crea y rellena la estructura de un plano. La función devulve un
// puntero a dicho plano.
// Lo primero es chequear la longitud del char **, si es distinto de 4, devolvemos
// NULL. Alojamos la memoria necesaria, y empezamos a rellenar la estructura
// Vuelvo a usar la función norm(), para el vector normal del plano, así me
// aseguro de que el vector no está fuera de rango
// Finalmente devolvemos la referencia al plano que acabamos de crear
t_plane	*new_plane(char **data)
{
	t_plane	*plane;

	if (double_pointer_len(data) != 4)
		return (NULL);
	plane = malloc(sizeof(*plane));
	if (!plane)
		return (NULL);
	plane->type = PLANE;
	plane->point = ft_atov(data[1]);
	plane->normal = norm(ft_atov(data[2]));
	plane->color = ft_atoc(data[3]);
	return (plane);
}

// Función que calcula si un rayo interesecciona con un plano, devuelve 1 es caso
// afirmativo y 0 en caso negativo
// Realiamos el producto punto de la dirección normalizada del rayo y la normal 
// del plano. El producto punto entre los dos vectores mide la proyección de uno
// sobre el otro. De esa forma si den es cero significa que el rayo es paralelo
// al plano y por lo tanto no hay intersección
// t indica la distancia a lo largo de la dirección del rayo donde se produce la
// intersección. Se comprueba si la intersección calculada está más cerca que
// cualquier otra intersección antes calculada y si es mayor que EPSILON, para 
// evitar fallos de precisión.
// Si el rayo y la normal del plano se mueven en la misma dirección, se
// invierte la dirección para la correcta reflexión
// Actualizamos los registros del rayo y devolvemos 1. En otro caso devolvemos 0
int	hit_plane(t_ray *ray, t_plane *plane)
{
	double	t;
	double	den;
	
	den = dot(norm(ray->direction), plane->normal);
	if (!den)
		return (0);
	t = dot(sub(plane->point, ray->origin), plane->normal) / den;
	if (ray->record.t > t && t > EPSILON)
	{
		if (dot(ray->direction, plane->normal) > 0)
			plane->normal = scale(plane->normal, -1);
		ray->record.t = t;
		ray->record.p = at(*ray);
		ray->record.normal = plane->normal;
		ray->record.color = plane->color;
		ray->record.type = plane->type;
		ray->record.object = plane;
		return (1);
	}
	return (0);
}