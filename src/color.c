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
	b = (((float)(u & 0xFF) / 0xFF) * ((float)(v & 0xFF) / 0xFF)) * 0xFF;
    return ((r << 0x10) | (g << 0x08) | b);
}

// Función que suman dos colores y devuelve la suma
// Básicamente aplicamos la operación suma y chequeamos que el valor sea correcto
// Dicha función fuerza a que el valor sea correcto y esté dentro de los límites
t_color	cadd(int color_a, int color_b)
{
	int	r;
	int	g;
	int	b;

	r = ccheck((color_a >> 0x10) + (color_b >> 0x10));
	g = ccheck((color_a >> 0x08 & 0xFF) + (color_b >> 0x08 & 0xFF));
	b = ccheck((color_a >> 0xFF) + (color_b >> 0xFF));
	return ((r << 0x10) | (g << 0x08) | b);
}

// Función que multiplica un color por un valor con decimales
// Como es posible que la multiplicación por un escalar provoque que el nunero
// resultante sea > 255 o < 0, hay que pasarle la función ccheck()
t_color	cscale(int color, float a)
{
	int r;
	int g;
	int b;

	r = ccheck(color * (color >> 0x10));
	g = ccheck(color * ((color >> 0x08) & 0xFF));
	b = ccheck(color * (color >> 0xFF));
	return ((r << 0x10) | (g << 0x08) | b);
}

// Función para calcular el color resultante de la intersección de la luz
// con el objetoen de la escena
// Esta función tendrá en cuenta el color del objeto y la intensidad y color
// de la luz
// Inicializamos el color en negro, obtenemos el color del objeto desde la
// estructura record. Escalamos el color del objeto con la intensidad de la
// luz y lo sumamos con el color que teníamos.
// Finalmente multiplicamos ambos colores, el color del objeto y el color de la luz
t_color	color_component(t_light *light, t_hit record)
{
	t_color	obj_color;
	t_color	color;

	color = 0x0;
	obj_color = record.color;
	color = cadd(color, cscale(obj_color, light_intensity(light, record)));
	color = cproduct(color, light->color);
	return(color);
}