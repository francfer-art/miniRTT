#include <minirt.h>

// Función que crea, rellena y devuelve un puntero a la estructura triángulo, desde un char **.
// Lo primero que hacemos es comprobar su longitud, si es distinta de 5 devolvemos NULL. Luego
// alojamos dinámicamente la memoria necesaria para almacenar el puntero a la estructura.
// Finalmente empezamos a rellenar los campos de la estructura y devolvemos el puntero.
t_triangle	*new_triangle(char **data)
{
	t_triangle	*triangle;
	t_vector	*v;

	if (double_pointer_len(data) != 5)
		return (NULL);
	triangle = malloc(sizeof(*triangle));
	if (!triangle)
		return (NULL);
	v = triangle->vertex;
	triangle->type = TRIANGLE;
	triangle->color = ft_atoc(data[4]);
	triangle->vertex[0] = ft_atov(data[3]);
	triangle->vertex[1] = ft_atov(data[2]);
	triangle->vertex[2] = ft_atov(data[1]);
	triangle->normal = norm(cross(sub(v[1], v[0]), sub(v[2], v[0])));
	return (triangle);
}

// Función que chequea si un rayo intersecciona con un triángulo. Recibe un rayo, un triángulo y
// devuelve 1 si hay intersección y 0 si no la hay.
// El algoritmo funciona de la siguiente manera:
// La función primero configura un plano que corresponde al triángulo utilizando el primer
// vértice del triángulo como un punto en el plano y su normal como la normal del plano.
// La función hit_plane verifica si el rayo intersecta este plano. Si lo hace, la función
// luego verifica si el punto de intersección se encuentra dentro del triángulo utilizando
// la función is_inside.
// Si el punto está dentro del triángulo, se actualiza el registro del rayo con la información
// de la intersección, y la función retorna 1.
// Si el punto está fuera del triángulo, o si el rayo no intersecta el plano en absoluto,
// la función retorna 0.
int	hit_triangle(t_ray *ray, t_triangle *triangle)
{
	int		hit;
	t_ray	r;
	t_plane	pl;

	hit = false;
	pl.point = triangle->vertex[0];
	pl.normal = triangle->normal;
	pl.color = triangle->color;
	r.direction = ray->direction;
	r.origin = ray->origin;
	r.record.t = ray->record.t;
	if (hit_plane(&r, &pl) && is_inside(r.record, triangle->vertex, 3))
	{
		*ray = r;
		ray->record.type = TRIANGLE;
		hit = true;
	}
	return (hit);
}
