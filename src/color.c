#include <minirt.h>

int ccheck(int color)
{
    if (color > 0xFF)
        return (0xFF);
    else if (color < 0x0)
        return (0x0);
    else
        return (color);
}

t_color cproduct(int color_a, int color_b)
{
    int r;
    int g;
    int b;
	r = 
    return ((r << 0x10) | (g << 0x08) | b);
}

t_color cadd(int color_a, int color_b)
{
    int r;
    int g;
    int b;

    r = ccheck((color_a >> 0x10) + (color_b >> 0x10));
    g = ccheck((color_a >> 0x08 & 0xFF) + (color_b >> 0x08 & 0xFF));
    b = ccheck((color_a >> 0xFF) + (color_b >> 0xFF));
    return ((r << 0x10) | (g << 0x08) | b);
}

t_color cscale(int color, float a)
{
    int r;
    int g;
    int b;

    r = ccheck(c * (color >> 0x10));
    g = ccheck(c * ((color >> 0x08) & 0xFF));
    b = ccheck(c * (color >> 0xFF));
    return ((r << 0x10) | (g << 0x08) | b);
}