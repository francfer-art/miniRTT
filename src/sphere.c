#include <minirt.h>

// Función que crea y rellena una esfera y devuelve un puntero a dicha estructura
// Lo primero que hacemos es comprobar la longitud del puntero doble, si la 
// longitud es distinta de 4 mostramos un error devolvemos NULL.
// Ahora alojamos dinámicamente un puntero a dicha estructura, y empezamos a
// rellenar la estructura.
// Finalmente devolvemos el puntero a dicha variable
t_sphere	*new_sphere(char **data)
{
	t_sphere *sphere;

	if (double_pointer_len(data) != 4)
		return (NULL);
	sphere = malloc(sizeof(*sphere));
	if (!sphere)
		return (NULL);
	sphere->type = SPHERE;
	sphere->color = ft_atoc(data[3]);
	sphere->radius = ft_atof(data[2]);
	sphere->center = ft_atov(data[1]);
	return (sphere);
}

// Función para calcular las raíces que se generan entre una esfera y un rayo
// Recibe de argumentos el rayo, la esfera y un array de floats, donde se almacenarán
// las raíces
// Lo primero que hacemos es calcular oc, es un vector que representa la distancia
// y dirección del centro de la esfera al origen del rayo
// a, half-b y c son los coeficientes de la escuación cuadrática
// Calculamos el discriminante de la ecuación cuadrática y procedemos a calcular
// las dos raíces
void	sphere_roots(t_ray ray, t_sphere sphere, float *root)
{
	t_vector	oc;
	float		a;
	float		half_b;
	float		c;
	float		sqrt_disc;

	oc = sub(ray.origin, sphere.center);
	a = length_square(ray.direction);
	half_b = dot(oc, ray.direction);
	c = length_square(oc) - pow(sphere.radius, 2);
	sqrt_disc = sqrt(pow(half_b, 2) - a * c);
	root[0] = (-half_b - sqrt_disc) / a;
	root[1] = (-half_b + sqrt_disc) / a;
}

// Función que determina si un rayo ha interseccionado con una esfera, devuelve
// 1 si hay intersección y 0 si no la hay.
// Lo primero que hacemos es calcular las raíces, llamando a sphere_roots
// La función verifica si la raíz actual root[i] es un valor positivo 
// (lo que significa que la intersección está delante del origen del rayo, no detrás)
// y si es menor que ray->record.t (lo que indica que es la intersección más cercana 
// al origen del rayo que se ha encontrado hasta ahora).
// Actualizamos los registros del rayo y devolvemos 1. En caso de que la raíz
// no sea válida iteraremos por el bucle. Si no se ha devulto 1 en ninguna vuelta, 
// devolvemos 0.
int	hit_sphere(t_ray *ray, t_sphere *sphere)
{
	unsigned int	i;
	float			root[2];

	i = 0;
	sphere_roots(*ray, *sphere, root);
	while (i < 2)
	{
		if (ray->record.t > root[i] && root[i] > 0)
		{
			ray->record.t = root[i];
			ray->record.p = at(*ray);
			ray->record.normal = norm(sub(ray->record.p, sphere->center));
			ray->record.color = sphere->color;
			ray->record.type = SPHERE;
			return (1);
		}
		i++;
	}
	return (0);
}