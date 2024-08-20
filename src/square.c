#include <minirt.h>

// función que calcula los vértices de un cuadrado. Recibe como argumento un 
// puntero a un cuadrado, la función rellenará el campo vértice del cuadrado
// dado como argumento.
// Las variables basis representan las bases del cuadrado
// El vector basis_i se calcula como el producto cruzado del vector normal, 
// con otro vector obtenido permutando las coordenadas del vector normal --> De
// esa forma se obtiene una de las direcciones para definir el plano del cuadrado
// Calculamos el basis_j como el producto cruzado del vector normal y el vector
// basis_i. Escalamos ambos vectores por la mitad del lado del cuadrado, porque
// cada vértice esta a la mitad de distancia desde el centro del cuadrado
// Finalmente calculamos los vértices del cuadrado
void    square_vertex(t_square *square)
{
    t_vector    basis_i;
    t_vector    basis_j;

    basis_i = cross(square->normal, vector(square->normal.z, square->normal.x, square->normal.y));
    basis_j = cross(square->normal, basis_i);
    basis_i = scale(basis_i, square->side / 2);
    basis_j = scale(basis_j, square->side / 2);
    square->vertex[0] = add(add(square->center, basis_i), basis_j);
    square->vertex[1] = sub(add(square->center, basis_i), basis_j);
    square->vertex[2] = sub(sub(square->center, basis_i), basis_j);
    square->vertex[3] = add(sub(square->center, basis_i), basis_j);
}

// Función para crear, rellenar y devolver un puntero a la estructura t_square. Recibe un char **
// del que extraerá la información. Lo primero es verificar la longitud de data, si es distinta de 5
// devolvemos NULL. Alojamos dinámicamente la memoria para el puntero. Rellenamos la estructura con
// los datos del char **, incluídos los vértices y devolvemos el cuadrado.
// Como siempre cuando obtenemos el vector normal, le aplicamos la función norm(), para normalizarlo,
// asegurándonos de que su módulo sea 1.
t_square    *new_square(char **data)
{
    t_square    *square;

    if (double_pointer_len(data) != 5)
        return (NULL);
    square = malloc(sizeof(*square));
    if (!square)
        return (NULL);
    square->type = SQUARE;
    square->center = ft_atov(data[1]);
    square->normal = norm(ft_atov(data[2]));
    square->side = ft_atof(data[3]);
    square->color = ft_atoc(data[4]);
    square_vertex(square);
    return(square);

}

static int is_inside(t_hit r, t_vector *v, int vertex)
{
    int				i;
    int             in;
    float           det[vertex];
    t_vector        det_cross;

    in = true;
    i = 0;
    while (i < vertex)
    {
        if (i == vertex - 1)
            det_cross = cross(sub(v[i], v[0]), sub(r.p, v[0]));
        else
            det_cross = cross(sub(v[i], v[i + 1]), sub(r.p, v[i + 1]));
        det[i] = dot(det_cross, r.normal);
        if (i > 0)
		{
			if ((det[i - 1] >= 0 && det[i] < 0) || (det[i - 1] < 0 && det[i] >= 0))
				in = 0;
		}
		i++;
    }
}

// Función que devuelve 1 si un rayo intersecciona con un cuadrado. Dicha función devuelve 0 si no hay
// intersección. Básicamente crearemos un plano (plane) que coincidirá con el cuadrado, para luego
// determinar si el rayo intersecciona con dicho plano.
// La variable hit es la variable que va a ser devuelta, la iniciamos en 0 y si 
// se produce la intersección la colocamos en 1.
// Colocamos el punto del plano, el vector normal y el color con los parámetros del
// cuadrado, hacemos lo mismo con el rayo.
// Si se produce una intersección y está dentro de los límites del cuadrado, nos 
// quedamos con el rayo, guardamos el objeto con el que ha interseccionado y 
// colocamos hit en 1.
// Finalmente devolvemos hit.
int	hit_square(t_ray *ray, t_square *square)
{
	int		hit;
	t_ray	r;
	t_plane	plane;

	hit = 0;
	plane.point = square->center;
	plane.normal = square->normal;
	plane.color = square->color;
	r.origin = ray->origin;
	r.direction = ray->direction;
	r.record.t = ray->record.t;
	if (hit_plane(&r, &plane) && is_inside(r.record, square->vertex, 4))
	{
		*ray = r;
		ray->record.object = square;
		hit = 1;
	}
	return (hit);
}
