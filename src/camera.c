#include <minirt.h>

// Función para inicializar las cámras de una lista de cámaras.
// vp es un array de floats que almacenan los valores de proyección de la ventana
// en las  direcciones horizontal y vertical. Los vectores u, v y w representan
// los ejes de la cámara.
// Calculamos el ancho de la proyección usando la tangente del fov dividido por 2 
// Calculamos la altura de la proyección usando el ancho y manteniendo el mismo ratio
// Normalizamos el vector de dirección de la cámara y lo asignamos a w
// Calculamos el vector u, que es perpendicular a w y al eje horizontal
// Calculamos v como vector perpendicular a u y w
// Escalamos los vectores v y u a los valores de la proyección
// Finalmente calculamos la esquina inferior izquierda y pasamos a la siguiente
// para seguir iterando
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

// Función para cambiar de cámara, recibe como argumento el server que contiene
// todos los datos y un entero que indica por cuántas cámaras vamos a iterar
// Siempre se va a renderizar desde la primera cámara, por tanto, el objetivo es
// actualizar la lista de modo cuando se renderice el server, la cámara haya 
// cambiado
void    change_camera(t_server *server, int code)
{
    t_list  **camset;
    int     step;

    camset = &(server->world->cameras);
    if (code == XK_c)
        step = 1;
    else
        step = -1;
    while(step-- > 0 && (*camset)->next)
        *camset = (*camset)->next;
    while(step++ < 0 && (*camset)->prev)
        *camset = ((*camset)->prev);
}

// Función que me genera un rayo a partir de una cámara hacia un punto específico
// de la pantalla. Lo primero que hacemos es declarar el rayo que vamos a devolver,
// así como variables propias del rayo
// Asignamos los valores iniciales y procedemos a  calcular la dirección del rayo
// Escalamos el vector horizontal y vertical por u y v para determinar el punto 
// correspondiente de u,v en pantalla
// Se suma este vector escalado a la esquina inferior izquierda (llc), finalmente
// restamos la dirección menos el origen, para obtener ese vector desde el origen
// Por último devolvemos el rayo
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

// La idea de esta función es que podamos movernos con la cámara seleccionada
// por la escena. Recibe como argumento el server , que contiene toda la información
// y un código. Dicho código nos indicará en qué dirección nos vamos a mover.
// Por ejemplo, si el código es XK_Left, la cámara se moverá hacia la izquierda
// Tengo en mente que el movimiento por la escena sea fluido, lo mismo tengo que
// recurrir a un render más básico mientras el usuario se mueve, y aplicar el
// general con un botón cuando el usuario termine de desplazarse.
void    move_camera(t_server *server, int code)
{
    t_camera    *camera;
    t_vector    right;
    t_vector    up;
    float       move_speed;

    camera = server->world->cameras->content;
    right = cross(camera->direction, vector(0, 1, 0));
    up = vector(0, 1 ,0);
    move_speed = 0.1;
    if (code == XK_Left)
    {
        ft_printf("Me muevo a la izquierda!\n");
        camera->origin = sub(camera->origin, scale(right, move_speed));    }
    else if (code == XK_Right)
    {
        ft_printf("Me muevo a la derecha!\n");;
        camera->origin = add(camera->origin, scale(right, move_speed));
    }
    else if (code == XK_Up)
    {
        ft_printf("Me muevo arriba!\n");;
        camera->origin = add(camera->origin, scale(camera->direction, move_speed));
    }
    else if (code == XK_Down)
    {
        ft_printf("Me muevo abajo!\n");;
        camera->origin = sub(camera->origin, scale(camera->direction, move_speed));
    }
}
