#include <minirt.h>

// Función que parsea y crea un cilindro desde un char **data
// Este doble puntero viene de hacer un split de cada string del archivo.rt 
// que nos pasan por argumento
// Lo primero es verificar la cantidad de elementos del dataset, si no son 6 error
// Reservo la cantidad de memoria necesaria para el objeto
// Empiezo a rellenar la estructura
// Cuando relleno el axis del cilindro, llamo a la función norm despues de llamar a 
// ft_atov. De este modo consigo que el vector esté normalizado, independientemente
// del vector que introduzcan en el archivo.rt
t_cylinder	*new_cylinder(char **data)
{
	t_cylinder	*cylinder;

	if (double_pointer_len(data) != 6)
		return (NULL);
	cylinder = malloc(sizeof(*cylinder));
	if (!cylinder)
		return (NULL);
	cylinder->type = CYLINDER;
	cylinder->color = ft_atoc(data[5]);
	cylinder->height = ft_atof(data[4]);
	cylinder->radius = ft_atof(data[3]) / 2;
	cylinder->axis = norm(ft_atov(data[2]));
	cylinder->center = ft_atov(data[1]);
	return (cylinder);
}

// Función para calcular las raíces de un cilindro, recibe como argumento el rayo, 
// el cilindro y un array de dos posiciones donde irán las raíces en caso de existir
// Proyectamos la dirección del rayo sobre el eje del cilindro
// Poyección de la diferencia entre el origen del rayo y el centro del cilindro sobre
// el eje del cilindro
// Calculo de las ecuaciones cuadráticas
// Resolución de las ecuaciones cuadráticas
void	cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2])
{
	float		a;
	float		half_b;
	float		c;
	t_vector	v;
	t_vector	w;

	v = scale(cylinder.axis, dot(r.direction, cylinder.axis));
	v = sub(r.direction, v);
	w = scale(cylinder.axis, (dot(sub(r.origin, cylinder.center),
					cylinder.axis)));
	w = sub(sub(r.origin, cylinder.center), v);
	a = length_square(v);
	half_b = dot(v, w);
	c = length_square(w) - pow(cylinder.radius, 2);
	roots[0] = (-half_b + sqrt(pow(half_b, 2) - a * c)) / a;
	roots[1] = (-half_b - sqrt(pow(half_b, 2) - a * c)) / a;
}

// Función que se encarga de seleccionar los valores t y d que más adecuada de 
// dos posibles intersecciones entre un rayo y un objeto. La función decide cuál
// de las dos intersecciones es válida o más cercana.
void	valid_hit(int *hit, float *dist, float *root, float *d, float *t)
{
	if (hit[0] && hit[1])
	{
		if (root[0] < root[1])
		{
			*d = dist[0];
			*t = root[0];
		}else
		{
			*d = dist[1];
			*t = root[1];
		}
	}
	else if (hit[0])
	{
		*d = dist[0];
		*t = root[0];		
	}
	else if (hit[hit[1]])
	{
		*d = dist[1];
		*t = root[1];
	}
}

float	solve(t_ray r, t_cylinder cy, float *d, int hit[2])
{
	float		t;
	float		dist[2];
	float		root[2];
	t_vector	b_ray;

	cylinder_roots(r, cy, root);
	b_ray = sub(cy.center, r.origin);
	dist[0] = dot(cy.axis, sub(scale(r.direction, root[0]), b_ray));
	dist[1] = dot(cy.axis, sub(scale(r.direction, root[1]), b_ray));
	hit[0] = (dist[0] >= 0 && dist[0] <= cy.height && root[0] > EPSILON);
	hit[1] = (dist[1] >= 0 && dist[1] <= cy.height && root[1] > EPSILON);
	valid_hit(hit, dist, root, d, &t);
	return (t);
}

int	hit_cylinder(t_ray *ray, t_cylinder *cylinder)
{
	float	y;
	float	t;
	int		hit[2];

	t = solve(*ray, *cylinder, &y, hit);
	if ((hit[0] || hit[1]) && ray->record.t > t && t > EPSILON)
	{
		ray->record.t = t;
		ray->record.p = at(*ray);
		ray->record.normal = norm(sub(ray->record.p, add(scale(cylinder->axis, y), cylinder->center)));
		if (hit[1] && !hit[0])
			ray->record.normal = scale(ray->record.normal, -1);
		ray->record.color = cylinder->color;
	}
	return (hit[0] || hit[1]);
}
