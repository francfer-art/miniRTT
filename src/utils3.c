#include <minirt.h>

t_vector	negate(t_vector v)
{
	return (vector(-v.x, -v.y, -v.z));
}

float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

t_vector	reflect_vector(t_vector v, t_vector normal)
{
	return (sub(v, scale(normal, 2.0 * dot(v, normal))));
}

void	ft_swap(float *a, float *b)
{
	float	c;

	c = *a;
	*a = *b;
	*b = c;
}

t_vector	refract_vector(t_vector v, t_vector normal, float ior,
		float env_ior)
{
	float		cosi;
	float		etai;
	float		etat;
	float		k;
	t_vector	n;

	cosi = clamp(dot(v, normal), -1.0, 1.0);
	etai = env_ior;
	etat = ior;
	n = normal;
	if (cosi < 0)
		cosi = -cosi;
	else
	{
		ft_swap(&etai, &etat);
		n = negate(normal);
	}
	k = 1.0 - (etai / etat) * (etai / etat) * (1.0 - cosi * cosi);
	if (k < 0)
		return (vector(0, 0, 0));
	return (add(scale(v, (etai / etat)), scale(n, ((etai / etat) * cosi
					- sqrt(k)))));
}
