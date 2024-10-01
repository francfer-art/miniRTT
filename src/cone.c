#include <minirt.h>


// Función para resolver una ecuación cuadrática
void solve_quadratic(float a, float b, float c, float *roots, int *root_count) {
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        *root_count = 2;
        roots[0] = (-b + sqrt(discriminant)) / (2 * a);
        roots[1] = (-b - sqrt(discriminant)) / (2 * a);
    } else if (discriminant == 0) {
        *root_count = 1;
        roots[0] = -b / (2 * a);
    } else {
        *root_count = 0;
    }
}

// Función para calcular las raíces de la intersección de un rayo con un cono
void cone_roots(t_ray ray, t_cone cone, float *roots, int *root_count) {
    t_vector oc = sub(ray.origin, cone.vertex);  // Vector desde el vértice del cono al origen del rayo

    float cos_theta_sq = cos(cone.angle) * cos(cone.angle);  // cos^2(θ)

    // Coeficientes de la ecuación cuadrática para la intersección
    float a = dot(ray.direction, cone.direction) * dot(ray.direction, cone.direction) - cos_theta_sq * dot(ray.direction, ray.direction);
    float b = 2 * (dot(ray.direction, cone.direction) * dot(oc, cone.direction) - cos_theta_sq * dot(ray.direction, oc));
    float c = dot(oc, cone.direction) * dot(oc, cone.direction) - cos_theta_sq * dot(oc, oc);

    solve_quadratic(a, b, c, roots, root_count);
}

// Función para calcular la normal del cono en un punto de intersección
t_vector cone_normal(t_cone cone, t_vector point) {
    t_vector v = sub(point, cone.vertex);  // Vector desde el vértice del cono al punto de intersección
    t_vector axis_component = scale(cone.direction, dot(v, cone.direction));  // Componente a lo largo del eje del cono
    t_vector normal = sub(v, scale(axis_component, cos(cone.angle) * cos(cone.angle)));  // Normal sin normalizar
    return norm(normal);  // Normalizar el vector normal
}

// Función que determina si un rayo ha intersectado con un cono, considerando ambas mitades
int hit_cone(t_ray *ray, t_cone *cone) {
    unsigned int i;
    float roots[2];  // Hasta 2 raíces posibles
    int root_count;

    cone_roots(*ray, *cone, roots, &root_count);
    for (i = 0; i < root_count; i++) {
        if (ray->record.t > roots[i] && roots[i] > EPSILON) {
            t_vector point = add(ray->origin, scale(ray->direction, roots[i]));
            
            // Calcula la distancia del punto de intersección al vértice del cono proyectada en la dirección del eje
            float dist_to_vertex = dot(sub(point, cone->vertex), cone->direction);

            // Comprueba si el punto de intersección está dentro de la altura del cono
            if (fabs(dist_to_vertex) <= cone->height) {
                ray->record.t = roots[i];
                ray->record.p = point;
                ray->record.normal = cone_normal(*cone, ray->record.p);
                ray->record.color = cone->color;
                ray->record.type = CONE;
                return 1;
            }
        }
    }
    return 0;
}

// Función para crear un nuevo cono a partir de los datos de entrada
t_cone *new_cone(char **data) {
    t_cone *cone;

    if (double_pointer_len(data) != 6)
        return (NULL);

    cone = malloc(sizeof(*cone));
    if (!cone)
        return (NULL);

    cone->type = CONE;
    cone->vertex = ft_atov(data[1]);
    cone->direction = norm(ft_atov(data[2]));  // Normaliza la dirección
    cone->angle = ft_atof(data[3]) * M_PI / 180;  // Convierte el ángulo a radianes
    cone->height = ft_atof(data[4]);  // Altura del cono
    cone->color = ft_atoc(data[5]);

    return (cone);
}
