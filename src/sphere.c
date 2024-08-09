#include <minirt.h>

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
}

int	hit_sphere(t_ray *ray, t_sphere *sphere)
{

}