#include <minirt.h>

typedef struct s_torus {
    t_vector center;  // Centro del toroide
    float major_radius; // Radio mayor (desde el centro del toroide al centro del tubo)
    float minor_radius; // Radio menor (del tubo en sí)
    t_color color;    // Color del toroide
    int type;         // Tipo de objeto, en este caso, TORUS
} t_torus;


t_torus *new_torus(char **data)
{
    t_torus *torus;

    if (double_pointer_len(data) != 5)  // Se espera 5 argumentos: comando, centro, radio mayor, radio menor, color
        return (NULL);
    torus = malloc(sizeof(*torus));
    if (!torus)
        return (NULL);
    torus->type = TORUS;
    torus->color = ft_atoc(data[4]);
    torus->major_radius = ft_atof(data[2]);
    torus->minor_radius = ft_atof(data[3]);
    torus->center = ft_atov(data[1]);
    return (torus);
}

void torus_roots(t_ray ray, t_torus torus, float *root)
{
    // Aquí calculamos las raíces de la ecuación de cuarto grado
    // basándonos en las propiedades del toroide y el rayo
    // Definimos los coeficientes de la ecuación de cuarto grado
    float a, b, c, d, e;
    
    // Calcula el coeficiente de la ecuación de cuarto grado
    // Suponemos que las funciones necesarias como `dot`, `pow` y otras
    // ya están definidas en la librería usada
    a = pow(dot(ray.direction, ray.direction), 2);
    b = 4 * dot(ray.direction, ray.origin) * dot(ray.direction, ray.direction);
    c = 2 * (dot(ray.direction, ray.direction) * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)))) + 4 * pow(dot(ray.direction, ray.origin), 2) + 4 * pow(torus.major_radius, 2) * pow(ray.direction.z, 2);
    d = 4 * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2))) * dot(ray.direction, ray.origin) + 8 * pow(torus.major_radius, 2) * ray.origin.z * ray.direction.z;
    e = pow(dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)), 2) - 4 * pow(torus.major_radius, 2) * (pow(ray.origin.z, 2));

    // Usar una función de resolución de ecuaciones de cuarto grado para calcular las raíces
    solve_quartic_equation(a, b, c, d, e, root);
}

int hit_torus(t_ray *ray, t_torus *torus)
{
    unsigned int i;
    float root[4];  // Hasta 4 raíces posibles

    i = 0;
    torus_roots(*ray, *torus, root);
    while (i < 4)
    {
        if (ray->record.t > root[i] && root[i] > 0)
        {
            ray->record.t = root[i];
            ray->record.p = at(*ray);
            ray->record.normal = norm(sub(sub(ray->record.p, torus->center), 
                                 scale(torus->major_radius, normalize(sub(ray->record.p, torus->center)))));
            ray->record.color = torus->color;
            ray->record.type = TORUS;
            return (1);
        }
        i++;
    }
    return (0);
}


// typedef struct s_cone {
//     t_vector vertex;       // Vértice del cono
//     t_vector direction;    // Dirección del eje del cono (debe estar normalizada)
//     float angle;           // Ángulo de apertura del cono (en radianes)
//     t_color color;         // Color del cono
//     int type;              // Tipo de objeto, en este caso, CONE
// } t_cone;

// t_cone *new_cone(char **data)
// {
//     t_cone *cone;

//     if (double_pointer_len(data) != 5)  // Se espera 5 argumentos: comando, vértice, dirección, ángulo, color
//         return (NULL);
//     cone = malloc(sizeof(*cone));
//     if (!cone)
//         return (NULL);
//     cone->type = CONE;
//     cone->color = ft_atoc(data[4]);
//     cone->angle = ft_atof(data[3]);
//     cone->direction = norm(ft_atov(data[2]));  // Normaliza la dirección
//     cone->vertex = ft_atov(data[1]);
//     return (cone);
// }

// // Función para resolver una ecuación cuadrática ax^2 + bx + c = 0 manualmente
// void solve_quadratic_manual(float a, float b, float c, float *roots, int *root_count)
// {
//     float discriminant = b * b - 4 * a * c;

//     if (discriminant > 0)  // Dos raíces reales
//     {
//         *root_count = 2;
//         roots[0] = (-b + sqrt(discriminant)) / (2 * a);
//         roots[1] = (-b - sqrt(discriminant)) / (2 * a);
//     }
//     else if (discriminant == 0)  // Una raíz real
//     {
//         *root_count = 1;
//         roots[0] = -b / (2 * a);
//     }
//     else  // No hay raíces reales
//     {
//         *root_count = 0;
//     }
// }


// void cone_roots(t_ray ray, t_cone cone, float *root)
// {
//     t_vector oc = sub(ray.origin, cone.vertex);  // Vector desde el vértice del cono al origen del rayo

//     float cos_theta_sq = cos(cone.angle) * cos(cone.angle);  // cos^2(θ)
//     float sin_theta_sq = sin(cone.angle) * sin(cone.angle);  // sin^2(θ)

//     float a = dot(ray.direction, cone.direction) * dot(ray.direction, cone.direction) - cos_theta_sq * dot(ray.direction, ray.direction);
//     float b = 2 * (dot(ray.direction, cone.direction) * dot(oc, cone.direction) - cos_theta_sq * dot(ray.direction, oc));
//     float c = dot(oc, cone.direction) * dot(oc, cone.direction) - cos_theta_sq * dot(oc, oc);

//     int root_count;
//     solve_quadratic(a, b, c, root, &root_count);
// }

// int hit_cone(t_ray *ray, t_cone *cone)
// {
//     unsigned int i;
//     float root[2];  // Hasta 2 raíces posibles

//     i = 0;
//     cone_roots(*ray, *cone, root);
//     while (i < 2)
//     {
//         if (ray->record.t > root[i] && root[i] > 0)
//         {
//             ray->record.t = root[i];
//             ray->record.p = at(*ray);
//             t_vector v = sub(ray->record.p, cone->vertex);
//             ray->record.normal = norm(sub(v, scale(dot(v, cone->direction), cone->direction)));
//             ray->record.color = cone->color;
//             ray->record.type = CONE;
//             return (1);
//         }
//         i++;
//     }
//     return (0);
// }
