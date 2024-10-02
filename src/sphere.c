#include <minirt.h>

t_sphere	*new_sphere(char **data)
{
	t_sphere	*sphere;

	if (double_pointer_len(data) != 4)
		return (NULL);
	sphere = malloc(sizeof(*sphere));
	if (!sphere)
		return (NULL);
	sphere->type = SPHERE;
	sphere->color = ft_atoc(data[3]);
	sphere->radius = ft_atof(data[2]);
	sphere->center = ft_atov(data[1]);
	sphere->material.type = 1;
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
	sqrt_disc = sqrt(pow(half_b, 2) - a * c);
	root[0] = (-half_b - sqrt_disc) / a;
	root[1] = (-half_b + sqrt_disc) / a;
}

void	fill_glass_material(t_ray *ray)
{
	ray->record.material.diffuse = 0x0;
	ray->record.material.specular = 0x4C3C3C;
	ray->record.material.shininess = 164;
	ray->record.material.reflectivity = 0.2;
	ray->record.material.refractivity = 1.0;
	ray->record.material.ior = 1.54;
}

void	fill_mate_material(t_ray *ray)
{
	ray->record.material.diffuse = 0x0;
	ray->record.material.specular = 0xFFFFFF;
	ray->record.material.shininess = 24;
	ray->record.material.reflectivity = 0.0;
	ray->record.material.refractivity = 0.0;
	ray->record.material.ior = 1.0;
}

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
			if (sphere->material.type)
				fill_glass_material(ray);
			else
				fill_mate_material(ray);
			return (1);
		}
		i++;
	}
	return (0);
}
