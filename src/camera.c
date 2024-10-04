#include <minirt.h>

void	init_cameras(t_list *camset, int width, int height)
{
	float		vp[2];
	t_camera	*camera;
	t_vector	u;
	t_vector	v;
	t_vector	w;

	while (camset)
	{
		camera = (t_camera *)camset->content;
		vp[0] = tan(camera->fov / 2) * 2;
		vp[1] = vp[0] * (float)height / width;
		camera->direction = scale(camera->direction, -1);
		w = norm(camera->direction);
		u = cross(vector(0, 1, 0), w);
		v = cross(w, u);
		camera->horizontal = scale(u, vp[0]);
		camera->vertical = scale(v, vp[1]);
		camera->llc = sub(camera->origin, scale(camera->horizontal, 0.5));
		camera->llc = sub(camera->llc, scale(camera->vertical, 0.5));
		camera->llc = sub(camera->llc, w);
		camset = camset->next;
	}
}

void	change_camera(t_server *server, int code)
{
	t_list	**camset;
	int		step;

	camset = &(server->world->cameras);
	if (code == XK_c)
		step = 1;
	else
		step = -1;
	if (step > 0)
	{
		while (step-- && (*camset)->next)
			*camset = (*camset)->next;
	}
	else if (step < 0)
	{
		while (step++ && (*camset)->prev)
			*camset = ((*camset)->prev);
	}
}

t_ray	generate_ray(t_camera *camera, float u, float v)
{
	t_ray		ray;
	t_vector	horizontal;
	t_vector	vertical;
	t_vector	llc;

	ft_memset(&ray, 0, sizeof(t_ray));
	horizontal = camera->horizontal;
	vertical = camera->vertical;
	llc = camera->llc;
	ray.origin = camera->origin;
	ray.direction = add(scale(horizontal, u), scale(vertical, v));
	ray.direction = add(ray.direction, llc);
	ray.direction = norm(sub(ray.direction, ray.origin));
	ray.record.object = NULL;
	return (ray);
}

void	move_camera(t_server *server, int code)
{
	t_camera	*camera;
	float		zoom_speed;

	camera = server->world->cameras->content;
	zoom_speed = 0.15;
	if (code == XK_ZOOM_IN)
		camera->origin = sub(camera->origin, scale(camera->direction,
					zoom_speed));
	else if (code == XK_ZOOM_OUT)
		camera->origin = add(camera->origin, scale(camera->direction,
					zoom_speed));
	recalculate_camera(camera, server->width, server->height);
}
