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
}

t_ray   generate_ray(t_camera *camera, float u, float v)
{
    t_ray       ray;
    t_vector    horizontal;
    t_vector    vertical;
    t_vector    llc;

    horizontal = camera->horizontal;
    vertical = camera->vertical;
    llc = camera->llc;
    ray.origin = camera->origin;
    ray.direction = add(scale(horizontal, u), scale(vertical, v));
    ray.direction = add(ray.direction, llc);
    ray.direction = norm(sub(ray.direction, ray.origin));
    ray.record.object = NULL;
    ray.record.color = 0x0;
    return (ray);
}

void    move_camera(t_server *server, int code)
{
    //Moveré la camara en función del código
    //Seleccionaré por el codigo y me moveré a la izquierda/derecha o arriba/abajo
    //Tengo que tener claro que camara estoy usando y modificar esa
}
