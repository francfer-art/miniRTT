#include <minirt.h>


// Función para resolver una ecuación cuadrática
// void solve_quadratic(float a, float b, float c, float *roots, int *root_count) {
//     float discriminant = b * b - 4 * a * c;

//     if (discriminant > 0) {
//         *root_count = 2;
//         roots[0] = (-b + sqrt(discriminant)) / (2 * a);
//         roots[1] = (-b - sqrt(discriminant)) / (2 * a);
//     } else if (discriminant == 0) {
//         *root_count = 1;
//         roots[0] = -b / (2 * a);
//     } else {
//         *root_count = 0;
//     }
// }

// Función para resolver una ecuación cuártica usando el método de Ferrari
void solve_quartic(float a, float b, float c, float d, float e, float *roots, int *root_count) {
    if (a == 0) {
        *root_count = 0;
        return;
    }

    b /= a;
    c /= a;
    d /= a;
    e /= a;

    float alpha = -3 * b * b / 8 + c;
    float beta = b * b * b / 8 - b * c / 2 + d;
    float gamma = -3 * pow(b / 4, 4) + (c * b * b) / 16 - (b * d) / 4 + e;

    float quadratic_roots[2];
    int quadratic_root_count;

    solve_quadratic(1, alpha, gamma - beta * beta / (4 * alpha), quadratic_roots, &quadratic_root_count);

    if (quadratic_root_count == 0) {
        *root_count = 0;
        return;
    }

    float p = sqrt(quadratic_roots[0]);
    float q = sqrt(quadratic_roots[1]);

    float quadratic1_roots[2], quadratic2_roots[2];
    int quadratic1_count, quadratic2_count;

    solve_quadratic(1, p, beta / (2 * p), quadratic1_roots, &quadratic1_count);
    solve_quadratic(1, -p, beta / (2 * p), quadratic2_roots, &quadratic2_count);

    *root_count = 0;
    for (int i = 0; i < quadratic1_count; i++) {
        roots[(*root_count)++] = quadratic1_roots[i] - b / 4;
    }
    for (int i = 0; i < quadratic2_count; i++) {
        roots[(*root_count)++] = quadratic2_roots[i] - b / 4;
    }
}

// Función para calcular las raíces de la intersección de un rayo con un toroide
void torus_roots(t_ray ray, t_torus torus, float *root) {
    float a, b, c, d, e;

    t_vector p = sub(ray.origin, torus.center);
    float dot_dir_dir = dot(ray.direction, ray.direction);
    float dot_p_dir = dot(p, ray.direction);
    float dot_p_p = dot(p, p);
    float R = torus.major_radius;
    float r = torus.minor_radius;

    a = pow(dot_dir_dir, 2);
    b = 4 * dot_dir_dir * dot_p_dir;
    c = 2 * dot_dir_dir * (dot_p_p - (R * R + r * r)) + 4 * pow(dot_p_dir, 2) + 4 * R * R * ray.direction.z * ray.direction.z;
    d = 4 * (dot_p_p - (R * R + r * r)) * dot_p_dir + 8 * R * R * p.z * ray.direction.z;
    e = pow(dot_p_p - (R * R + r * r), 2) - 4 * R * R * pow(p.z, 2);

    int root_count;
    solve_quartic(a, b, c, d, e, root, &root_count);
}

// Función para calcular la normal en un punto del toroide
t_vector torus_normal(t_torus torus, t_vector point) {
    t_vector to_point = sub(point, torus.center);
    t_vector projected = sub(to_point, scale(norm(to_point), torus.major_radius));
    return norm(projected);
}

// Función que determina si un rayo ha intersectado con un toroide
int hit_torus(t_ray *ray, t_torus *torus) {
    unsigned int i;
    float root[4];  // Hasta 4 raíces posibles

    i = 0;
    torus_roots(*ray, *torus, root);
    while (i < 4) {
        if (ray->record.t > root[i] && root[i] > 0) {
            ray->record.t = root[i];
            ray->record.p = add(ray->origin, scale(ray->direction, ray->record.t));
            ray->record.normal = torus_normal(*torus, ray->record.p);
            ray->record.color = torus->color;
            ray->record.type = TORUS;
            return (1);
        }
        i++;
    }
    return (0);
}

// Función para crear un nuevo toroide a partir de los datos de entrada
t_torus *new_torus(char **data) {
    t_torus *torus;

    if (double_pointer_len(data) != 5)
        return (NULL);

    torus = malloc(sizeof(*torus));
    if (!torus)
        return (NULL);

    torus->type = TORUS;
    torus->center = ft_atov(data[1]);
    torus->axis = (t_vector){0, 1, 0};
    torus->major_radius = ft_atof(data[2]);
    torus->minor_radius = ft_atof(data[3]);
    torus->color = ft_atoc(data[4]);

    return (torus);
}

// typedef struct s_torus {
//     t_vector center;  // Centro del toroide
//     float major_radius; // Radio mayor (desde el centro del toroide al centro del tubo)
//     float minor_radius; // Radio menor (del tubo en sí)
//     t_color color;    // Color del toroide
//     int type;         // Tipo de objeto, en este caso, TORUS
// } t_torus;


// t_torus *new_torus(char **data)
// {
//     t_torus *torus;
//     printf("here!\n");
//     // Verifica que haya exactamente 5 argumentos (comando, centro, radio mayor, radio menor, color)
//     if (double_pointer_len(data) != 5)  
//         return (NULL);

//     // Asigna memoria para el nuevo toroide
//     torus = malloc(sizeof(*torus));
//     if (!torus)
//         return (NULL);

//     // Inicializa los campos de la estructura del toroide
//     torus->type = TORUS;                           // Define el tipo de objeto
//     torus->center = ft_atov(data[1]);              // Convierte el vector del centro desde la cadena
// 	printf("data[1] = %s\n", data[1]);
//     torus->axis = (t_vector){0, 1, 0};             // Define el eje del toroide (por defecto, alineado al eje Y)
//     torus->major_radius = ft_atof(data[2]);        // Convierte el radio mayor desde la cadena
// 	printf("data[2] = %s\n", data[2]);
//     torus->minor_radius = ft_atof(data[3]);        // Convierte el radio menor desde la cadena
// 	printf("data[3] = %s\n", data[3]);
//     torus->color = ft_atoc(data[4]);               // Convierte el color desde la cadena
// 	printf("data[4] = %s\n", data[4]);

//     // Retorna el puntero al nuevo toroide
//     return (torus);
// }


// void solve_quadratic(float a, float b, float c, float *roots, int *root_count)
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

// void solve_quartic(float a, float b, float c, float d, float e, float *roots, int *root_count)
// {
//     if (a == 0)
//     {
//         // Si a es 0, entonces no es una ecuación cuártica. Podrías usar solve_cubic si quieres manejar el caso cúbico.
//         *root_count = 0;
//         return;
//     }

//     // Normalizamos los coeficientes dividiendo por 'a'
//     b /= a;
//     c /= a;
//     d /= a;
//     e /= a;

//     // Definimos los términos intermedios
//     float alpha = -3 * b * b / 8 + c;
//     float beta = b * b * b / 8 - b * c / 2 + d;
//     float gamma = -3 * (b / 4) * (b / 4) * (b / 4) * (b / 4) + c * b * b / 16 - b * d / 4 + e;

//     float quadratic_roots[2];
//     int quadratic_root_count;

//     // Resolvemos la ecuación cuadrática auxiliar
//     solve_quadratic(1, alpha, gamma - beta * beta / (4 * alpha), quadratic_roots, &quadratic_root_count);

//     if (quadratic_root_count == 0)
//     {
//         *root_count = 0; // No hay raíces reales
//         return;
//     }

//     float p = sqrt(quadratic_roots[0]);
//     float q = sqrt(quadratic_roots[1]);

//     // Resolvemos las dos ecuaciones cuadráticas resultantes
//     float quadratic1_roots[2], quadratic2_roots[2];
//     int quadratic1_count, quadratic2_count;

//     solve_quadratic(1, p, beta / (2 * p), quadratic1_roots, &quadratic1_count);
//     solve_quadratic(1, -p, beta / (2 * p), quadratic2_roots, &quadratic2_count);

//     // Almacenar las raíces encontradas en el array de resultados
//     *root_count = 0;
//     for (int i = 0; i < quadratic1_count; i++)
//     {
//         roots[(*root_count)++] = quadratic1_roots[i] - b / 4;
//     }
//     for (int i = 0; i < quadratic2_count; i++)
//     {
//         roots[(*root_count)++] = quadratic2_roots[i] - b / 4;
//     }
// }


// // void solve_quartic(float a, float b, float c, float d, float e, float *roots, int *root_count)
// // {
// //     if (a == 0)
// //     {
// //         // Si a es 0, entonces no es una ecuación cuártica. Podrías usar solve_cubic si quieres manejar el caso cúbico.
// //         *root_count = 0;
// //         return;
// //     }

// //     // Normalizamos los coeficientes dividiendo por 'a'
// //     b /= a;
// //     c /= a;
// //     d /= a;
// //     e /= a;

// //     // Definimos los términos intermedios
// //     float alpha = -3 * b * b / 8 + c;
// //     float beta = b * b * b / 8 - b * c / 2 + d;
// //     float gamma = -3 * (b / 4) * (b / 4) * (b / 4) * (b / 4) + c * b * b / 16 - b * d / 4 + e;

// //     float quadratic_roots[2];
// //     int quadratic_root_count;

// //     // Resolvemos la ecuación cuadrática auxiliar
// //     solve_quadratic(1, alpha, gamma - beta * beta / (4 * alpha), quadratic_roots, &quadratic_root_count);

// //     if (quadratic_root_count == 0)
// //     {
// //         *root_count = 0; // No hay raíces reales
// //         return;
// //     }

// //     float p = sqrt(quadratic_roots[0]);
// //     float q = sqrt(quadratic_roots[1]);

// //     // Resolvemos las dos ecuaciones cuadráticas resultantes
// //     float quadratic1_roots[2], quadratic2_roots[2];
// //     int quadratic1_count, quadratic2_count;

// //     solve_quadratic(1, p, beta / (2 * p), quadratic1_roots, &quadratic1_count);
// //     solve_quadratic(1, -p, beta / (2 * p), quadratic2_roots, &quadratic2_count);

// //     // Almacenar las raíces encontradas en el array de resultados
// //     *root_count = 0;
// //     for (int i = 0; i < quadratic1_count; i++)
// //     {
// //         roots[(*root_count)++] = quadratic1_roots[i] - b / 4;
// //     }
// //     for (int i = 0; i < quadratic2_count; i++)
// //     {
// //         roots[(*root_count)++] = quadratic2_roots[i] - b / 4;
// //     }
// // }

// // void torus_roots(t_ray ray, t_torus torus, float *root)
// // {
// //     float a, b, c, d, e;
    
// //     // Calcula los coeficientes de la ecuación de cuarto grado
// //     a = pow(dot(ray.direction, ray.direction), 2);
// //     b = 4 * dot(ray.direction, ray.origin) * dot(ray.direction, ray.direction);
// //     c = 2 * (dot(ray.direction, ray.direction) * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)))) + 4 * pow(dot(ray.direction, ray.origin), 2) + 4 * pow(torus.major_radius, 2) * pow(ray.direction.z, 2);
// //     d = 4 * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2))) * dot(ray.direction, ray.origin) + 8 * pow(torus.major_radius, 2) * ray.origin.z * ray.direction.z;
// //     e = pow(dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)), 2) - 4 * pow(torus.major_radius, 2) * (pow(ray.origin.z, 2));

// //     int root_count;
// //     solve_quartic(a, b, c, d, e, root, &root_count);
// // }

// void torus_roots(t_ray ray, t_torus torus, float *root)
// {
//     float a, b, c, d, e;
    
//     // Calcula los coeficientes de la ecuación de cuarto grado
//     t_vector p = sub(ray.origin, torus.center); // Vector del rayo desde el centro del toroide
//     float dot_dir_dir = dot(ray.direction, ray.direction);
//     float dot_p_dir = dot(p, ray.direction);
//     float dot_p_p = dot(p, p);
//     float R = torus.major_radius;
//     float r = torus.minor_radius;

//     // Coeficientes para la ecuación cuártica
//     a = dot_dir_dir * dot_dir_dir;
//     b = 4 * dot_dir_dir * dot_p_dir;
//     c = 2 * dot_dir_dir * (dot_p_p - (R * R + r * r)) + 4 * dot_p_dir * dot_p_dir + 4 * R * R * ray.direction.z * ray.direction.z;
//     d = 4 * (dot_p_p - (R * R + r * r)) * dot_p_dir + 8 * R * R * p.z * ray.direction.z;
//     e = (dot_p_p - (R * R + r * r)) * (dot_p_p - (R * R + r * r)) - 4 * R * R * (p.z * p.z);

//     int root_count;
//     solve_quartic(a, b, c, d, e, root, &root_count);
// }


// // void torus_roots(t_ray ray, t_torus torus, float *root)
// // {
// //     // Aquí calculamos las raíces de la ecuación de cuarto grado
// //     // basándonos en las propiedades del toroide y el rayo
// //     // Definimos los coeficientes de la ecuación de cuarto grado
// //     float a, b, c, d, e;
    
// //     // Calcula el coeficiente de la ecuación de cuarto grado
// //     // Suponemos que las funciones necesarias como `dot`, `pow` y otras
// //     // ya están definidas en la librería usada
// //     a = pow(dot(ray.direction, ray.direction), 2);
// //     b = 4 * dot(ray.direction, ray.origin) * dot(ray.direction, ray.direction);
// //     c = 2 * (dot(ray.direction, ray.direction) * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)))) + 4 * pow(dot(ray.direction, ray.origin), 2) + 4 * pow(torus.major_radius, 2) * pow(ray.direction.z, 2);
// //     d = 4 * (dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2))) * dot(ray.direction, ray.origin) + 8 * pow(torus.major_radius, 2) * ray.origin.z * ray.direction.z;
// //     e = pow(dot(ray.origin, ray.origin) - (pow(torus.major_radius, 2) + pow(torus.minor_radius, 2)), 2) - 4 * pow(torus.major_radius, 2) * (pow(ray.origin.z, 2));

// //     // Usar una función de resolución de ecuaciones de cuarto grado para calcular las raíces
// //     solve_quartic_equation(a, b, c, d, e, root);
// // }

// // int hit_torus(t_ray *ray, t_torus *torus)
// // {
// //     unsigned int i;
// //     float root[4];  // Hasta 4 raíces posibles

// //     i = 0;
// //     torus_roots(*ray, *torus, root);
// //     while (i < 4)
// //     {
// //         if (ray->record.t > root[i] && root[i] > 0)
// //         {
// //             ray->record.t = root[i];
// //             ray->record.p = at(*ray);
// //             ray->record.normal = norm(sub(sub(ray->record.p, torus->center), 
// //                                  scale(norm(sub(ray->record.p, torus->center)), torus->major_radius)));
// //             ray->record.color = torus->color;
// //             ray->record.type = TORUS;
// //             return (1);
// //         }
// //         i++;
// //     }
// //     return (0);
// // }

// t_vector torus_normal(t_torus torus, t_vector point)
// {
//     // Vector desde el centro del toroide al punto de intersección
//     t_vector to_point = sub(point, torus.center);

//     // Proyecta en el plano del toroide (resta la componente a lo largo del eje del toroide)
//     t_vector projected = sub(to_point, scale(norm(to_point), torus.major_radius));

//     // Calcula la normal final normalizando el vector proyectado
//     return norm(projected);
// }


// int hit_torus(t_ray *ray, t_torus *torus)
// {
//     unsigned int i;
//     float root[4];  // Hasta 4 raíces posibles

//     i = 0;
//     torus_roots(*ray, *torus, root);
//     while (i < 4)
//     {
//         if (ray->record.t > root[i] && root[i] > 0)
//         {
//             ray->record.t = root[i];
//             ray->record.p = at(*ray);  // Calcula el punto de intersección

//             // Calcula la normal en el punto de intersección
//             ray->record.normal = torus_normal(*torus, ray->record.p);

//             ray->record.color = torus->color;
//             ray->record.type = TORUS;
//             return (1);
//         }
//         i++;
//     }
//     return (0);
// }


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
