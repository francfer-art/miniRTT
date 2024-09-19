#include <minirt.h>

// Devolverá 1 si hay alguna intersección con algún objeto de la escena,
// y devolverá 0 si no hay intersección. Recibe como argumento un rayo y una lista
// enlazada donde cada nodo es una figura
// Lo primero que hacemos es inicializar los valores de hit en 0 y la r en INFINITO
// para asegurarnos de que la intersección encontrada sea la más cercana.
// Iteramos mientras exista nuestra lista enlazada. En cada iteración obtenemos el
// tipo de la figura y llamamos a la función de intersección correspondiente
// Cuando la función acabe se devuelve 1 o 0, lo importante es que mientras vamos 
// iterando la estructura rayo se actualiza si hay intersección y si la distancia
// a dicha intersección es más cercana de la que había antes. Por eso es importante
// inicializar la distancia t a INFINITO al principio del bucle.
int intersec(t_ray *ray, t_list *figures)
{
    int     hit;
    t_type  type;

    hit = 0;
    ray->record.t = INFINITY;
    while (figures)
    {
        type = *((t_type *)figures->content);
        if (type == CYLINDER)
            hit |= hit_cylinder(ray, (t_cylinder *)figures->content);
        else if (type == PLANE)
            hit |= hit_plane(ray, (t_plane *)figures->content);
        else if (type == SPHERE)
            hit |= hit_sphere(ray, (t_sphere *)figures->content);
        else if (type == SQUARE)
            hit |= hit_square(ray, (t_square *)figures->content);
        else if (type == TRIANGLE)
            hit |= hit_triangle(ray, (t_triangle *)figures->content);
        figures = figures->next;
    }
    return (hit);
}

t_color checkerboard_square(t_ray *ray)
{
	int		    check_x;
	int		    check_y;
	int         checker;
	float	    size;
    t_square    *square;

    square = ray->record.object;
	size = square->side;
	check_x = (int)floor(ray->record.p.z / size);
	check_y = (int)floor(ray->record.p.x / size);
	checker = (check_x + check_y) % 2;
	if (checker == 0)
		return ray->record.color;
	else
		return (0xFFFFFF - ray->record.color);
}


t_color	checkerboard_sphere(t_ray *ray, float size)
{
    int		check_x;
    int		check_y;
    int		check_z;
    int		checker;

	check_x = (int)floor(ray->record.p.x / size);
	check_y = (int)floor(ray->record.p.y / size);
	check_z = (int)floor(ray->record.p.z / size);
	checker = (check_x + check_y + check_z) % 2;
	if (checker == 0)
		return ray->record.color;
	else
		return (0xFFFFFF - ray->record.color);
}

t_color checkerboard_cylinder(t_ray *ray)
{
    t_cylinder  *cylinder;
    float       u;
    float       v;
    float       theta;

    cylinder = ray->record.object;
    theta = atan2(ray->record.p.z / cylinder->radius, ray->record.p.x / cylinder->radius);
    u = (theta + M_PI) / (2 * M_PI);
    v = ray->record.p.y / cylinder->radius;
    if (v <= 0)
        v += 1.0;
    if (!((((int)floor(u * cylinder->radius) + (int)floor(v * cylinder->radius)) % 2)))
        return (ray->record.color);
    else
        return (0xFFFFFF - ray->record.color);
}

t_color checkerboard_pattern_selector(t_ray *ray)
{
	if (ray->record.type == SPHERE)
		return (checkerboard_sphere(ray, 1.0));
	else if (ray->record.type == PLANE)
		return (checkerboard_sphere(ray, 3.0));
	else if (ray->record.type == SQUARE)
		return (checkerboard_square(ray));
	else if (ray->record.type == CYLINDER)
		return (checkerboard_cylinder(ray));
	else
		return (checkerboard_sphere(ray, 1.0));
	return (ray->record.color);
}

void    treat_material(t_ray *ray, t_world *world, int depth)
{
    t_color reflected_color;
    t_color refracted_color;
    
    if (ray->record.type == SPHERE || ray->record.type == PLANE || ray->record.type == SQUARE)
    {
        reflected_color = reflect(ray, world, depth);
        ray->record.color = cadd(ray->record.color, cscale(reflected_color, ray->record.material.reflectivity));
    }
    if (ray->record.type == SPHERE || ray->record.type == PLANE || ray->record.type == SQUARE)
    {
        refracted_color = refract(ray, world, depth);
        ray->record.color = cadd(ray->record.color, cscale(refracted_color, ray->record.material.refractivity));
    }
}

t_color get_texture_color(t_texture *texture, float u, float v)
{
    int tex_x;
    int tex_y;
    int pixel_index;
    
    tex_x = (int)(u * texture->width) % texture->width;
    tex_y = (int)(v * texture->height) % texture->height;
    pixel_index = (tex_y * texture->size_line) + (tex_x * (texture->bpp / 8));
    return (*(int*)(texture->img_data + pixel_index));
}

t_color phong_lighting(t_light light, t_ray *ray)
{
    t_color     color;
    t_vector    light_dir;
    t_vector    view_dir;
    t_vector    reflect_dir;

    float spec;
    float diffuse_factor;
    float specular_factor = 3;
    int shininess = 3;
    color = 0x0;
    light_dir = norm(sub(light.position, ray->record.p));
    diffuse_factor = fmax(dot(ray->record.normal, light_dir), 0.0);
    color = cadd(color, cscale(ray->record.material.diffuse, diffuse_factor));
    view_dir = norm(sub(ray->origin, ray->record.p));
    reflect_dir = reflect_vector(negate(light_dir), ray->record.normal);
    spec = pow(fmax(dot(view_dir, reflect_dir), 0.0), 3);
    color = cadd(color, cscale(ray->record.material.specular, spec * 3));
    color = cscale(color, light_intensity(light, ray->record));
    color = cproduct(color, light.color);
    return (color);
}

float get_height_from_texture(t_texture *texture, float u, float v)
{
    int     tex_color;
    int     RED_channel;
    int     GREEN_channel;
    int     blue_channel;
    float   average_intensity;

    tex_color = get_texture_color(texture, u, v);
    RED_channel = (tex_color >> 16) & 0xFF;
    GREEN_channel = (tex_color >> 8) & 0xFF;
    blue_channel = tex_color & 0xFF;
    average_intensity = (RED_channel + GREEN_channel + blue_channel) / 3.0;
    return (average_intensity / 255.0);
}

t_vector calculate_tangent(t_vector normal)
{
    t_vector tangent;

    if (fabs(normal.y) > 0.9)
        tangent = vector(1, 0, 0);
    else
        tangent = vector(0, 1, 0);
    tangent = cross(tangent, normal);
    return (norm(tangent));
}

t_vector bump_function(t_hit record, t_texture *texture, float u, float v)
{
    float height_center;
    float height_u;
    float height_v;
    float d_height_u;
    float d_height_v;

    height_center = get_height_from_texture(texture, u, v);
    height_u = get_height_from_texture(texture, u + 50.0 / texture->width, v);
    height_v = get_height_from_texture(texture, u, v + 1200.0 / texture->height);
    d_height_u = (height_u - height_center) * 7;
    d_height_v = (height_v - height_center) * 7;
    record.normal = add(record.normal, scale(calculate_tangent(record.normal), d_height_u));
    record.normal = add(record.normal, scale(cross(record.normal, calculate_tangent(record.normal)), d_height_v));
    return (norm(record.normal));
}


t_vector apply_bump(t_hit record, t_world *world, float u, float v)
{
    t_vector    perturbed_normal;
    t_vector    bump;
    
    perturbed_normal = record.normal;
    bump = bump_function(record, world->texture_img, u, v);
    perturbed_normal = add(perturbed_normal, bump);
    return (norm(perturbed_normal));
}

void    apply_texture_sphere(t_ray *ray, t_world *world, float *u, float *v)
{
    *u = 1 - (0.5 + (atan2(ray->record.normal.z, ray->record.normal.x) / (2 * M_PI)));
    *v = 0.5 - (asin(ray->record.normal.y) / M_PI);
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_square(t_ray *ray, t_world *world, float *u, float *v)
{
    t_square    *square;
    t_vector    u_axis;
    t_vector    v_axis;
    t_vector    relative_p;

    square = ray->record.object;
    if (fabs(square->normal.y) < 0.999)
        u_axis = (cross(square->normal, vector(0, 1, 0)));
    else
        u_axis = (cross(square->normal, vector(1, 0, 0)));
    v_axis = (cross(square->normal, u_axis));
    relative_p = sub(ray->record.p, square->center);
    *u = 1 - dot(relative_p, u_axis) / square->side + 0.5;
    *v = dot(relative_p, v_axis) / square->side + 0.5;
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_plane(t_ray *ray, t_world *world, float *u, float *v)
{
    t_plane     *plane;
    t_vector    u_axis;
    t_vector    v_axis;
    t_vector    relative_p;
    float       texture_scale;

    plane = ray->record.object;
    if (fabs(plane->normal.y) < 0.999)
        u_axis = cross(plane->normal, vector(0, 1, 0));
    else
        u_axis = cross(plane->normal, vector(1, 0, 0));
    v_axis = cross(plane->normal, u_axis);
    relative_p = sub(ray->record.p, plane->point);
    *u = dot(relative_p, u_axis);
    *v = dot(relative_p, v_axis);
    texture_scale = 100.0;
    *u = (*u / texture_scale) - floor(*u / texture_scale);
    *v = (*v / texture_scale) - floor(*v / texture_scale);
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture_cylinder(t_ray *ray, t_world *world, float *u, float *v)
{
    t_cylinder  *cylinder;
    t_vector    p_minus_base;
    t_vector    around_axis;
    float       height_along_axis;
    float       theta;

    cylinder = ray->record.object;
    p_minus_base = sub(ray->record.p, cylinder->center);
    height_along_axis = dot(p_minus_base, norm(cylinder->axis));
    if (height_along_axis < 0)
        height_along_axis = 0;
    if (height_along_axis > cylinder->height)
        height_along_axis = cylinder->height;
    *v = height_along_axis / cylinder->height;
    around_axis= sub(p_minus_base, scale(norm(cylinder->axis), height_along_axis));
    theta = atan2(around_axis.z, around_axis.x);
    *u = 1 - (theta / (2 * M_PI));
    ray->record.color = get_texture_color(world->texture_img, *u, *v);
}

void    apply_texture(t_ray *ray, t_world *world)
{
    float   u;
    float   v;
    if (world->texture)
    {
        u = 0;
        v = 0;
        if (ray->record.type == SPHERE)
            apply_texture_sphere(ray, world, &u, &v);
        else if (ray->record.type == SQUARE)
            apply_texture_square(ray, world, &u, &v);
        else if (ray->record.type == PLANE)
            apply_texture_plane(ray, world, &u, &v);
        else if (ray->record.type == CYLINDER)
            apply_texture_cylinder(ray, world, &u, &v);
        if (world->bump)
            ray->record.normal = apply_bump(ray->record, world, u, v);
    }
}

void    apply_material(t_ray *ray, t_world *world, int depth)
{
    if (world->material)
        treat_material(ray, world, depth);
}

void    apply_checkerboard(t_ray *ray, t_world *world)
{
    if (world->checkerboard)
        ray->record.color = checkerboard_pattern_selector(ray);
}

int check_depth_intersec(t_ray *ray, t_world *world, int depth)
{
    if (depth <= 0)
        return (0);
    if (!intersec(ray, world->figures))
        return (0);
    return (1);
}

// El raytracing se encarga de determinar la intersección del rayo
// con los objetos en la escena y calcular el color resultante basado
// en las propiedades del material, las luces, y otras consideraciones.
// Si no hay intersección con ningun objeto devuelvo color negro
t_color raytracer(t_ray *ray, t_world *world, int depth)
{
    int     vis;
    t_color color;
    t_color ambient;
    t_list  *light;
    t_light current_light;

    if (!check_depth_intersec(ray, world, depth))
        return (0x0);
    apply_texture(ray, world);
    apply_material(ray, world, depth);
    apply_checkerboard(ray, world);
    light = world->lights;
    ambient = cscale((*world->ambient).color, (*world->ambient).brightness);
    color = cproduct(ray->record.color, ambient);
    while(light)
    {
        current_light = *((t_light *)light->content);
        vis = !in_shadow(current_light, world->figures, ray->record);
        if (world->phong)
            color = cadd(color, phong_lighting(current_light, ray));
        else
            color = cadd(color, vis * color_component(current_light, ray->record));
        light = light->next;
    }
    return (color);
}

// La función render se encargará de renderizar la escena y mostrarla en una 
// ventana gráfica
// Si no hay cámaras, salimos del renderizado
// Hacemos un bucle anidado, idéntico al de fractol. Recorrerá todos los pixeles 
// de pantalla e irá :
//      Generando el rayo, calculando el color con la función raytracer() y poniendo en 
//      la imagen el pixel con el nuevo color
// Tras salir del bucle pondremos la imagen por pantalla mlx_put_image_to_window
// void    render(t_server *server)
// {
//     int     i;
//     int     j;
//     t_ray   ray;
//     t_color pixel_color;

//     if (!server->world->cameras)
//         return ;
//     j = server->height;
//     while (j-- > 0)
//     {
//         i = server->width;
//         while (i-- > 0)
//         {
//             ray = generate_ray(server->world->cameras->content, (float)i / server->width, (float)j / server->height);
//             pixel_color = raytracer(&ray, server->world);
//             my_put_pixel(server, i, server->height - 1 - j, pixel_color);
//         }
//     }
//     mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
// }

// Función que realiza el render en una sección delimitada. Dicha sección la realiza
// un hilo diferente. La idea es dividir la ventana en tantos cuadrados como hilos
// tengae el procesador
void *render_section(void *threadarg)
{
    t_thread_data   *data;
    int             i;
    int             j;
    t_color         pixel_color;
    t_ray           ray;
    int             max_depth;
    
    data = (t_thread_data *)threadarg;
    max_depth = 5;
    j = data->start_row;
    while (j < data->end_row)
    {
        i = 0;
        while (i < data->server->width)
        {
            ray = generate_ray(data->server->world->cameras->content, (float)i / data->server->width, (float)j / data->server->height);
            pixel_color = raytracer(&ray, data->server->world, max_depth);
            my_put_pixel(data->server, i, data->server->height - 1 - j, pixel_color);
            i++;
        }
        j++;
    }
    pthread_exit(NULL);
}

t_color average_color(t_color *colors, int num_colors)
{
    int r_sum;
    int g_sum;
    int b_sum;
    int i;
    r_sum = 0;
    g_sum = 0;
    b_sum = 0;
    i = -1;
    while(++i < num_colors)
    {
        r_sum += (colors[i] >> 16) & 0xFF;
        g_sum += (colors[i] >> 8) & 0xFF;
        b_sum += colors[i] & 0xFF;
    }
    return ((r_sum / num_colors) << 16) | ((g_sum / num_colors)) << 8 | (b_sum / num_colors);
}

void *render_section_super(void *threadarg)
{
    t_thread_data   *data;
    int             i, j, m, n;
    t_color         pixel_colors[4];
    t_color         sample_color;
    t_ray           ray;
    int             max_depth;
    int             samples_per_pixel = 4;
    int             sqrt_samples = 2;

    data = (t_thread_data *)threadarg;
    max_depth = 2;
    j = data->start_row;
    while (j < data->end_row)
    {
        i = 0;
        while (i < data->server->width)
        {
            for (int k = 0; k < samples_per_pixel; k++) {
                pixel_colors[k] = 0;
            }

            int k = 0;
            for (m = 0; m < sqrt_samples; m++)
            {
                for (n = 0; n < sqrt_samples; n++)
                {
                    
                    float u = ((float)i + (m + 0.5) / sqrt_samples) / data->server->width;
                    float v = ((float)j + (n + 0.5) / sqrt_samples) / data->server->height;
                    
                    ray = generate_ray(data->server->world->cameras->content, u, v);
                    sample_color = raytracer(&ray, data->server->world, max_depth);
                    
                    
                    pixel_colors[k] = sample_color;
                    k++;
                }
            }
            
            
            t_color averaged_color = average_color(pixel_colors, samples_per_pixel);

            my_put_pixel(data->server, i, data->server->height - 1 - j, averaged_color);
            i++;
        }
        j++;
    }
    pthread_exit(NULL);
}

void    show_menu(t_server *server)
{
    printf("-----------------------\n");
    if (server->world->checkerboard)
        printf("%sCheckerboard is on 🙿%s\n", GREEN, RESET);
    else
        printf("%sCheckerboard is off 🙿%s\n", RED, RESET);
    if (server->world->material)
        printf("%sMaterial is on 🔨%s\n", GREEN, RESET);
    else
        printf("%sMaterial is off 🔨%s\n", RED, RESET);
    if (server->world->phong)
        printf("%sPhong effect is on 🔦%s\n", GREEN, RESET);
    else
        printf("%sPhong effect is off 🔦%s\n", RED, RESET);
    if (server->world->bump)
        printf("%sBump texture is on 🧱%s\n", GREEN, RESET);
    else
        printf("%sBump texture is off 🧱%s\n", RED, RESET);
    if (server->world->texture)
        printf("%sTexture is on 🗺%s\n", GREEN, RESET);
    else
        printf("%sTexture is off 🗺%s\n", RED, RESET);
    printf("-----------------------\n");
}

// Función para renderizar la escena, es similar a la función render de arriba, pero
// el único cambio es que dividimos la escena por secciones. Cada seccion será
// renderizada por un hilo, consiguiendo agilizar la computación, lo que proporciona
// una mayor fluidez.
void render(t_server *server)
{
    pthread_t       threads[NUM_THREADS];
    t_thread_data   thread_data[NUM_THREADS];
    int             rows_per_thread;
    int             rc;
    int             t;
    
    rows_per_thread= server->height / NUM_THREADS;
    if (!server->world->cameras)
        return;
    t = -1;
    while (++t < NUM_THREADS)
    {
        thread_data[t].server = server;
        thread_data[t].start_row = t * rows_per_thread;
        if (t == NUM_THREADS - 1)
            thread_data[t].end_row = server->height;
        else
            thread_data[t].end_row = thread_data[t].start_row + rows_per_thread;
        if (server->world->material)
            rc = pthread_create(&threads[t], NULL, render_section_super, (void *)&thread_data[t]);
        else
            rc = pthread_create(&threads[t], NULL, render_section, (void *)&thread_data[t]);
        if (rc)
            full_message_exit(ERROR_CREATE, NULL, server);
    }
    t = -1;
    while (++t < NUM_THREADS)
        if (pthread_join(threads[t], NULL))
            full_message_exit(ERROR_JOIN, NULL, server);
    show_menu(server);
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}

// Función que rellanará un cuadrado de pixeles en lugar de 1 solo. EStá función es la 
// utilizada en la función render_low. 
void    fill_pixels(t_server *server, int scale_factor, int i, int j, t_color pixel_color)
{
    int x;
    int y;
    int orig_i;
    int orig_j;

    y = -1;
    while (++y < scale_factor)
    {
        x = -1;
        while (++x < scale_factor)
        {
            orig_i = i * scale_factor + x;
            orig_j = j * scale_factor + y;
            my_put_pixel(server, orig_i, server->height - 1 - orig_j, pixel_color);
        }
    }
}

int	adjust_scale_factor(t_server *server)
{
    int	area;
	int	scale_factor;

    area = server->width * server->height;
	scale_factor = area / BASE_FACTOR;
    if (scale_factor < 1)
        scale_factor = 5;
    return scale_factor;
}


// Función para que el renderizado sea mucho más pixelado y por tanto involucre 
// menos computación (idea de Chema). Básicamente en lugar de pintar 1 pixel e iterar
// por todos los pixeles de la pantalla, pintamos un cuadrado de pixeles del mismo
// color. Esta función sólo se usa cuando nos estamos moviendo por la pantalla, nos 
// interesa verlo todo fluido y cuando tengoamos la posición deseada pulsamos el 
// espacio, para  renderizar toda la escena.
void render_low(t_server *server)
{
    int     i;
    int     j;
	int		scale_factor;
    t_ray   ray;
    t_color pixel_color;
    int     max_depth;

    scale_factor = adjust_scale_factor(server);
    max_depth = 5;
    if (!server->world->cameras || scale_factor <= 0)
        return;
    j = -1;
    while (++j < server->height / scale_factor)
    {
        i = -1;
        while (++i < server->width / scale_factor)
        {
            ray = generate_ray(server->world->cameras->content, (float)i / (server->width / scale_factor), (float)j / (server->height / scale_factor));
            pixel_color = raytracer(&ray, server->world, max_depth);
            fill_pixels(server, scale_factor, i, j, pixel_color);
        }
    }
    mlx_put_image_to_window(server->mlx, server->window, server->image->image, 0, 0);
}
