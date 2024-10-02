#include <minirt.h>

void	update_ambient_color(t_server *server, int code)
{
	int	color;
	int	red;
	int	green;
	int	blue;

	color = server->world->ambient->color;
	red = (color >> 0x010) & 0xFF;
	green = (color >> 0x08) & 0xFF;
	blue = color & 0xFF;
	if (code == XK_r && red <= 250)
		red = red + 5;
	else if (code == XK_t && red >= 5)
		red = red - 5;
	else if (code == XK_g && green <= 250)
		green = green + 5;
	else if (code == XK_h && green >= 5)
		green = green - 5;
	else if (code == XK_b && blue <= 250)
		blue = blue + 5;
	else if (code == XK_n && blue >= 5)
		blue = blue - 5;
	color = (red << 0x010) | (green << 0x08) | blue;
	server->world->ambient->color = color;
}

void	update_texture(t_server *server)
{
	if (!server->world->texture)
	{
		server->world->texture = 1;
		server->world->material = 0;
		server->world->checkerboard = 0;
	}
	else
	{
		server->world->texture = 0;
		server->world->material = 0;
		server->world->checkerboard = 0;
	}
}

void	update_material(t_server *server)
{
	if (!server->world->material)
		server->world->material = 1;
	else
		server->world->material = 0;
}

void	update_checkerboard(t_server *server)
{
	if (!server->world->checkerboard)
	{
		server->world->checkerboard = 1;
	}
	else
		server->world->checkerboard = 0;
}

void	update_phong(t_server *server)
{
	if (!server->world->phong)
		server->world->phong = 1;
	else
		server->world->phong = 0;
}
