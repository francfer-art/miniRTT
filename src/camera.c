#include <minirt.h>

void    init_cameras(t_list *camset, int width, int height)
{
    float       vp[2];
    t_camera    *camera;
    t_vector    u;
    t_vector    v;
    t_vector    w;

    while(camset)
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

void    change_camera(t_server *server, int step)
{
    t_list  **camset;

    camset = &(server->world->cameras);
    while(step-- && (*camset)->next)
        *camset = (*camset)->next;
    render(server);
}