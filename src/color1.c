#include <minirt.h>

// Función para chequear que el color dado por argumento es correcto
// Si es mayor de 255 devuelve 255
// Si es menor que 0 devuelve 0+
// En caso de que esté en rango [0, 255] devuelve dicho valor
t_color ccheck(int color)
{
	if (color > 0xFF)
		return (0xFF);
	else if (color < 0x0)
		return (0x0);
	else
		return (color);
}

// Hay que hacer la multiplicación relativa para que el resultado te quede
// entre [0, 1] y luego multiplicarlo por 255(0xFF)
// Cabe destacar que con la minilib no hay elemento alpha, es decir el color es RGB
// y no RGBA como en la MLX42
t_color	cproduct(int color_a, int color_b)
{
	int	r;
	int	g;
	int	b;

	r = (((float)(color_a >> 0x10) / 0xFF) * ((float)(color_b >> 0x10) / 0xFF)) * 0xFF;
	g = (((float)((color_a >> 0x08) & 0xFF) / 0xFF) * ((float)((color_b >> 0x08) & 0xFF) / 0xFF)) * 0xFF;
	b = (((float)(color_a & 0xFF) / 0xFF) * ((float)(color_b & 0xFF) / 0xFF)) * 0xFF;
    return ((r << 0x10) | (g << 0x08) | b);
}