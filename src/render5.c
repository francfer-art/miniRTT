#include <minrt.h>

void render_pixel(t_thread_data *data, int i, int j)
{
    t_color pixel_colors[SAMPLES_PER_PIXEL];
    t_color averaged_color;
    
    initialize_colors(pixel_colors);
    compute_sample_colors(data, i, j, pixel_colors);
    averaged_color = average_color(pixel_colors, SAMPLES_PER_PIXEL);
    my_put_pixel(data->server, i, data->server->height - 1 - j, averaged_color);
}

void sample_pixel_colors(t_thread_data *data, t_color *pixel_colors, int i, int j)
{
    int m;
    int k;

    k = 0;
    m = 0;
    while (m < SQRT_SAMPLES)
    {
        k = sample_pixel_row(data, pixel_colors, i, j, m, k);
        m++;
    }
}

int sample_pixel_row(t_thread_data *data, t_color *pixel_colors, int i, int j, int m, int k)
{
    int n;

    n = 0;
    while (n < SQRT_SAMPLES)
    {
        k = sample_pixel(data, pixel_colors, i, j, m, n, k);
        n++;
    }
    return (k);
}

int sample_pixel(t_thread_data *data, t_color *pixel_colors, int i, int j, int m, int n, int k)
{
    t_ray   ray;
    t_color sample_color;
    float   u;
    float   v;

    u = calculate_u(data->server->width, i, m);
    v = calculate_v(data->server->height, j, n);
    ray = generate_ray(data->server->world->cameras->content, u, v);
    sample_color = raytracer(&ray, data->server->world, MAX_DEPTH);
    pixel_colors[k] = sample_color;
    k++;
    return (k);
}

float calculate_u(int width, int i, int m)
{
    float u;

    u = (float)i + ((float)m + 0.5) / (float)SQRT_SAMPLES;
    u = u / (float)width;
    return u;
}