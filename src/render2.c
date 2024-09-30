#include <minirt.h>

t_color	get_texture_color(t_texture *texture, float u, float v)
{
	int	tex_x;
	int	tex_y;
	int	pixel_index;

	tex_x = (int)(u * texture->width) % texture->width;
	tex_y = (int)(v * texture->height) % texture->height;
	pixel_index = (tex_y * texture->size_line) + (tex_x * (texture->bpp / 8));
	return (*(int *)(texture->img_data + pixel_index));
}

t_color	phong_lighting(t_light light, t_ray *ray)
{
	t_color		color;
	t_vector	vector[3];
	float		spec;
	float		diffuse_factor;

	color = 0x0;
	vector[0] = norm(sub(light.position, ray->record.p));
	diffuse_factor = fmax(dot(ray->record.normal, vector[0]), 0.0);
	color = cadd(color, cscale(ray->record.material.diffuse, diffuse_factor));
	vector[1] = norm(sub(ray->origin, ray->record.p));
	vector[2] = reflect_vector(negate(vector[0]), ray->record.normal);
	spec = pow(fmax(dot(vector[1], vector[2]), 0.0), 3);
	color = cadd(color, cscale(ray->record.material.specular, spec * 3));
	color = cscale(color, light_intensity(light, ray->record));
	color = cproduct(color, light.color);
	return (color);
}

float	get_height_from_texture(t_texture *texture, float u, float v)
{
	int		tex_color;
	int		red_channel;
	int		green_channel;
	int		blue_channel;
	float	average_intensity;

	tex_color = get_texture_color(texture, u, v);
	red_channel = (tex_color >> 16) & 0xFF;
	green_channel = (tex_color >> 8) & 0xFF;
	blue_channel = tex_color & 0xFF;
	average_intensity = (red_channel + green_channel + blue_channel) / 3.0;
	return (average_intensity / 255.0);
}

t_vector	calculate_tangent(t_vector normal)
{
	t_vector	tangent;

	if (fabs(normal.y) > 0.9)
		tangent = vector(1, 0, 0);
	else
		tangent = vector(0, 1, 0);
	tangent = cross(tangent, normal);
	return (norm(tangent));
}

t_vector	bump_function(t_hit record, t_texture *texture, float u, float v)
{
	float	height_center;
	float	height_u;
	float	height_v;
	float	d_height_u;
	float	d_height_v;

	height_center = get_height_from_texture(texture, u, v);
	height_u = get_height_from_texture(texture, u + 50.0 / texture->width, v);
	height_v = get_height_from_texture(texture, u, v + 1200.0
			/ texture->height);
	d_height_u = (height_u - height_center) * 7;
	d_height_v = (height_v - height_center) * 7;
	record.normal = add(record.normal, scale(calculate_tangent(record.normal),
				d_height_u));
	record.normal = add(record.normal, scale(cross(record.normal,
					calculate_tangent(record.normal)), d_height_v));
	return (norm(record.normal));
}
