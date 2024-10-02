#include <minirt.h>

t_color	cadd(int color_a, int color_b)
{
	int	r;
	int	g;
	int	b;

	r = ccheck((color_a >> 0x10) + (color_b >> 0x10));
	g = ccheck((color_a >> 0x08 & 0xFF) + (color_b >> 0x08 & 0xFF));
	b = ccheck((color_a & 0xFF) + (color_b & 0xFF));
	return ((r << 0x10) | (g << 0x08) | b);
}

t_color	cscale(int color, float a)
{
	int	r;
	int	g;
	int	b;

	r = ccheck(a * (color >> 0x10));
	g = ccheck(a * ((color >> 0x08) & 0xFF));
	b = ccheck(a * (color & 0xFF));
	return ((r << 0x10) | (g << 0x08) | b);
}

t_color	color_component(t_light light, t_hit record)
{
	t_color	obj_color;
	t_color	color;

	color = 0x0;
	obj_color = cadd(record.color, record.material.diffuse);
	color = cadd(color, cscale(obj_color, light_intensity(light, record)));
	color = cproduct(color, light.color);
	return (color);
}

int	in_shadow(t_light light, t_list *figures, t_hit record)
{
	t_ray	shadow;

	shadow.origin = add(record.p, scale(record.normal, EPSILON));
	shadow.direction = norm(sub(light.position, shadow.origin));
	shadow.record.object = record.object;
	return (intersec(&shadow, figures));
}

float	light_intensity(t_light light, t_hit record)
{
	t_vector	light_dir;
	float		gain;
	float		r2;

	light_dir = sub(light.position, record.p);
	r2 = length_square(light_dir);
	gain = dot(norm(light_dir), record.normal);
	if (gain <= 0)
		return (0);
	return ((light.brightness * gain * ALBEDO) / (4.0 * M_PI * r2));
}
