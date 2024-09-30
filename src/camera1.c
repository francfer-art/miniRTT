#include <minirt.h>

void	recalculate_camera(t_camera *camera, int width, int height)
{
	t_vector	u;
	t_vector	v;
	t_vector	w;
	float		vp[2];

	vp[0] = tan(camera->fov / 2) * 2;
	vp[1] = vp[0] * (float)height / width;
	w = norm(camera->direction);
	u = norm(cross(vector(0, 1, 0), w));
	v = cross(w, u);
	camera->horizontal = scale(u, vp[0]);
	camera->vertical = scale(v, vp[1]);
	camera->llc = sub(camera->origin, scale(camera->horizontal, 0.5));
	camera->llc = sub(camera->llc, scale(camera->vertical, 0.5));
	camera->llc = sub(camera->llc, w);
}

void	rotate_camera_horizontal(t_server *server, float angle)
{
	t_vector	new_direction;
	float		cos_angle;
	float		sin_angle;
	t_camera	*camera;

	camera = server->world->cameras->content;
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	new_direction.x = camera->direction.x * cos_angle - camera->direction.z
		* sin_angle;
	new_direction.z = camera->direction.x * sin_angle + camera->direction.z
		* cos_angle;
	new_direction.y = camera->direction.y;
	camera->direction = norm(new_direction);
}

void	rotate_camera_vertical(t_server *server, float angle)
{
	t_vector	new_direction;
	t_vector	right;
	float		cos_angle;
	float		sin_angle;
	t_camera	*camera;

	camera = server->world->cameras->content;
	cos_angle = cos(angle);
	sin_angle = sin(angle);
	right = norm(cross(vector(0, 1, 0), camera->direction));
	new_direction.x = camera->direction.x;
	new_direction.y = camera->direction.y * cos_angle - camera->direction.z
		* sin_angle;
	new_direction.z = camera->direction.y * sin_angle + camera->direction.z
		* cos_angle;
	camera->direction = norm(new_direction);
}

void	move_camera_position(t_server *server, int code)
{
	t_camera	*camera;
	t_vector	right;
	t_vector	up;
	float		move_speed;

	up = vector(0, 1, 0);
	move_speed = 0.1;
	camera = server->world->cameras->content;
	right = cross(camera->direction, up);
	if (code == XK_a)
		camera->origin = sub(camera->origin, scale(right, move_speed));
	else if (code == XK_d)
		camera->origin = add(camera->origin, scale(right, move_speed));
	else if (code == XK_w)
		camera->origin = add(camera->origin, scale(up, move_speed));
	else if (code == XK_s)
		camera->origin = sub(camera->origin, scale(up, move_speed));
	recalculate_camera(camera, server->width, server->height);
}

void	move_camera_rotate(t_server *server, int code)
{
	float	rotation_speed;

	rotation_speed = 0.05;
	if (code == XK_Left)
		rotate_camera_horizontal(server, -rotation_speed);
	else if (code == XK_Right)
		rotate_camera_horizontal(server, rotation_speed);
	else if (code == XK_Up)
		rotate_camera_vertical(server, rotation_speed);
	else if (code == XK_Down)
		rotate_camera_vertical(server, -rotation_speed);
	recalculate_camera(server->world->cameras->content, server->width,
		server->height);
}
