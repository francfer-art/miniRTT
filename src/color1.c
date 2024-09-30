#include <minirt.h>

t_color	ccheck(int color)
{
	if (color > 0xFF)
		return (0xFF);
	else if (color < 0x0)
		return (0x0);
	else
		return (color);
}

t_color	cproduct(int color_a, int color_b)
{
	int	r;
	int	g;
	int	b;

	r = (((float)(color_a >> 0x10) / 0xFF) * ((float)(color_b >> 0x10) / 0xFF))
		* 0xFF;
	g = (((float)((color_a >> 0x08) & 0xFF) / 0xFF)
			* ((float)((color_b >> 0x08) & 0xFF) / 0xFF)) * 0xFF;
	b = (((float)(color_a & 0xFF) / 0xFF) * ((float)(color_b & 0xFF) / 0xFF))
		* 0xFF;
	return ((r << 0x10) | (g << 0x08) | b);
}
