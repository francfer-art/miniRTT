#include <minirt.h>

t_cylinder  *new_cylinder(char **data)
{
    t_cylinder  *cylinder;

    if (double_pointer_len(data) != 6)
        return (NULL);
    cylinder = malloc(sizeof(*cylinder));
    if (!cylinder)
        return (NULL);
    cylinder->type = CYLINDER;
    cylinder->color = ft_atoc(data[5]);
    cylinder->height = ft_atof(data[4]);
    cylinder->radius = ft_atof(data[3]) / 2;
    cylinder->axis = ft_atov(data[2]);
    cylinder->center = ft_atov(data[1]);
    if (out_range_vector(cylinder->axis))
        message_exit(ERROR_VECTOR);
    else
        cylinder->axis = norm(cylinder->axis);
}

void    cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2])
{
    float       a;
    float       half_b;
    float       c;
    t_vector    v;
    t_vector    w;
}
