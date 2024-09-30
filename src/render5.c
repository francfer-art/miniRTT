#include <minirt.h>

void	render_pixel(t_thread_data *data, int i, int j)
{
	t_color	pixel_colors[SAMPLES_PER_PIXEL];
	t_color	averaged_color;

	initialize_colors(pixel_colors);
	compute_sample_colors(data, i, j, pixel_colors);
	averaged_color = average_color(pixel_colors, SAMPLES_PER_PIXEL);
	my_put_pixel(data->server, i, data->server->height - 1 - j, averaged_color);
}

float	calculate_u(int width, int i, int m)
{
	float	u;

	u = (float)i + ((float)m + 0.5) / (float)SQRT_SAMPLES;
	u = u / (float)width;
	return (u);
}
