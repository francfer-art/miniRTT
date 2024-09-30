#include <minirt.h>

void	update_hook(int keycode, t_server *server)
{
	if (keycode == XK_r || keycode == XK_g || keycode == XK_b || keycode == XK_t
		|| keycode == XK_h || keycode == XK_n)
		update_ambient_color(server, keycode);
	else if (keycode == XK_CHECKERBOARD)
		update_checkerboard(server);
	else if (keycode == XK_m)
		update_material(server);
	else if (keycode == XK_TEXTURE)
		update_texture(server);
	else if (keycode == XK_PHONG)
		update_phong(server);
	else if (keycode == XK_BUMP)
		update_bump(server);
}

static int	handle_camera_keys(int keycode, t_server *server)
{
	if (keycode == XK_Left || keycode == XK_Right || keycode == XK_Up
		|| keycode == XK_Down)
		move_camera_rotate(server, keycode);
	else if (keycode == XK_w || keycode == XK_s || keycode == XK_a
		|| keycode == XK_d)
		move_camera_position(server, keycode);
	else if (keycode == XK_c || keycode == XK_x)
		change_camera(server, keycode);
	else
		return (0);
	return (1);
}

static int	handle_effects_keys(int keycode, t_server *server)
{
	if (keycode == XK_r || keycode == XK_g || keycode == XK_b || keycode == XK_t
		|| keycode == XK_h || keycode == XK_n)
		update_ambient_color(server, keycode);
	else if (keycode == XK_CHECKERBOARD)
		update_checkerboard(server);
	else if (keycode == XK_m)
		update_material(server);
	else if (keycode == XK_TEXTURE)
		update_texture(server);
	else if (keycode == XK_PHONG)
		update_phong(server);
	else if (keycode == XK_BUMP)
		update_bump(server);
	else
		return (0);
	return (1);
}

int	key_press_hook(int keycode, t_server *server)
{
	if (keycode == XK_Escape)
		full_message_exit(ERROR_JEFE, NULL, server);
	else if (keycode == XK_SCREENSHOT)
		return (take_screenshot(server), 1);
	else if (keycode == XK_space)
	{
		render(server);
		return (1);
	}
	if (handle_camera_keys(keycode, server))
	{
		render_low(server);
		return (0);
	}
	if (handle_effects_keys(keycode, server))
	{
		render_low(server);
		return (0);
	}
	return (1);
}

int	mouse_handler(int button, int x, int y, t_server *server)
{
	x++;
	y++;
	if (button == XK_ZOOM_OUT)
		move_camera(server, button);
	else if (button == XK_ZOOM_IN)
		move_camera(server, button);
	else if (button == XK_LEFT_BUTTON)
		update_ambient_brightness(server, 0.05);
	else if (button == XK_RIGHT_BUTTON)
		update_ambient_brightness(server, -0.05);
	render_low(server);
	return (0);
}
