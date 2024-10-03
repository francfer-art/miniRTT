#ifndef MINIRT_H
# define MINIRT_H

# include <X11/X.h>
# include <X11/keysym.h>
# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <mlx.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

// Threads
# define NUM_THREADS 12
# define SAMPLES_PER_PIXEL 4
# define SQRT_SAMPLES 2
# define MAX_DEPTH 5

// File extension
# define EXTENSION ".rt"

// Window name
# define WINDOW_TITLE "miniRT"

// Variable for errors
# define EPSILON 0.001

// Factor de low render
# define BASE_FACTOR 50000

// Show menu colors
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define PURPLE "\033[1;35m"
# define YELLOW "\033[1;33m"
# define RESET "\033[0m"

// Some defines that are not in X11 library
# define XK_ZOOM_IN 0x0004
# define XK_ZOOM_OUT 0x0005
# define XK_LEFT_BUTTON 0x0001
# define XK_RIGHT_BUTTON 0x0003
# define XK_CHECKERBOARD 0XBA
# define XK_TEXTURE 0x0075
# define XK_PHONG 0x006C
# define XK_BUMP 0x0070
# define XK_SCREENSHOT 0xFF0D

// MATERIALS
# define ALBEDO 1000
# define ALBEDO_POLISHED_METAL 900
# define ALBEDO_GLASS 700
# define ALBEDO_PLASTIC 500
# define ALBEDO_MATTE 300
# define ALBEDO_NONE 0

typedef enum {
    ERROR_JEFE,
    // ERROR_MALLOC,
    ERROR_PARSE,
    ERROR_CREATE,
    ERROR_JOIN,
    ERROR_COLOR,
    // ERROR_SERVER,
    // ERROR_COL_VAL,
    // ERROR_VECTOR,
    ERROR_BRIGHTNESS,
    // ERROR_RATIO,
    ERROR_DIR,
    ERROR_ARG,
    ERROR_ID,
    ERROR_RES,
    ERROR_RES_LEN,
    ERROR_RES_VALUE,
    ERROR_AMB,
    ERROR_FORMAT,
    ERROR_OPEN,
    ERROR_CLOSE,
	ERROR_CAMERA,
    ERROR_COUNT
} ErrorType;

typedef int		t_color;

typedef enum s_type
{
	SPHERE,
	PLANE,
	SQUARE,
	TRIANGLE,
	CYLINDER,
	CONE
}				t_type;

typedef struct s_material
{
	int			type;
	t_color		diffuse;
	t_color		specular;
	float		shininess;
	float		reflectivity;
	float		refractivity;
	float		ior;
}				t_material;

typedef struct s_point
{
	double		x;
	double		y;
	double		z;
}				t_point;

typedef struct s_vector
{
	double		x;
	double		y;
	double		z;
}				t_vector;

typedef struct s_sphere
{
	t_type		type;
	t_vector	center;
	float		radius;
	t_color		color;
	t_material	material;
}				t_sphere;

typedef struct s_plane
{
	t_type		type;
	t_vector	point;
	t_vector	normal;
	t_color		color;
	t_material	material;
}				t_plane;

typedef struct s_quare
{
	t_type		type;
	t_vector	center;
	float		side;
	t_vector	normal;
	t_vector	vertex[4];
	t_color		color;
	t_material	material;
}				t_square;

typedef struct s_cylinder
{
	t_type		type;
	t_vector	center;
	t_vector	axis;
	float		radius;
	float		height;
	t_vector	edege[2];
	t_color		color;
}				t_cylinder;

typedef struct s_triangle
{
	t_type		type;
	t_vector	vertex[3];
	t_vector	normal;
	t_color		color;
}				t_triangle;

typedef struct s_cone {
    t_type type;
    t_vector vertex;
    t_vector direction;
    float angle;
    t_color color;
	float height;
	float a;
	float b;
	float c;
	int root_count;
} t_cone;

typedef struct s_camera
{
	t_vector	origin;
	t_vector	direction;
	t_vector	horizontal;
	t_vector	vertical;
	t_vector	llc;
	float		fov;
}				t_camera;

typedef struct s_light
{
	t_vector	position;
	float		brightness;
	t_color		color;
}				t_light;

typedef struct s_hit
{
	t_vector	normal;
	t_vector	p;
	float		t;
	void		*object;
	t_color		color;
	t_type		type;
	t_material	material;
}				t_hit;

typedef struct s_ray
{
	t_vector	origin;
	t_vector	direction;
	t_hit		record;
}				t_ray;

typedef struct s_texture
{
	void		*img_ptr;
	char		*img_data;
	int			width;
	int			height;
	int			bpp;
	int			size_line;
	int			endian;
}				t_texture;

typedef struct s_world
{
	int			*resolution;
	t_list		*figures;
	t_list		*lights;
	t_list		*cameras;
	t_light		*ambient;
	int			checkerboard;
	int			material;
	int			texture;
	t_texture	*texture_img;
	int			phong;
	int			bump;
}				t_world;

typedef struct s_image
{
	void		*image;
	char		*data;
	int			bpp;
	int			endian;
	int			size_line;
}				t_image;

typedef struct s_server
{
	void		*mlx;
	void		*window;
	t_image		*image;
	t_world		*world;
	int			width;
	int			height;
}				t_server;

typedef struct s_thread_data
{
	t_server	*server;
	int			start_row;
	int			end_row;
}				t_thread_data;

// error.c
void			message_exit(ErrorType msg);
void			msg_exit(ErrorType error);
void			full_message_exit(ErrorType msg, t_world *world, t_server *server);
void			check_server(t_server *server, int *t, int *rows_per_thread);
void			set_texture(t_world *world);

// scene.c
t_world			*scene_init(char *file);
int				open_scene_file(char *file);
t_world			*new_world(void);
void			init_texture(t_server *server, t_texture *texture,
					char *file_path);

// utils.c
int				double_pointer_len(char **data);
int				out_range_vector(t_vector v);
float			degree_2_rad(float degree);
float			ft_atof(char *str);
t_vector		ft_atov(char *str);

// utils1.c
void			free_double(char **s);
int				invalid_number(char *str);
t_color			ft_atoc(char *str);
t_vector		norm(t_vector v);
t_vector		scale(t_vector v, double c);

// utils2.c
double			sqr(double num);
double			length_square(t_vector v);
double			length(t_vector v);
double			dot(t_vector v, t_vector w);
t_vector		add(t_vector v, t_vector w);
t_vector		sub(t_vector v, t_vector w);
t_vector		at(t_ray ray);
t_vector		cross(t_vector v, t_vector w);
t_vector		negate(t_vector v);
float			clamp(float value, float min, float max);
t_vector		reflect_vector(t_vector v, t_vector normal);
t_vector		refract_vector(t_vector v, t_vector normal, float ior,
					float env_ior);
t_color			reflect(t_ray *ray, t_world *world, int depth);
t_color			refract(t_ray *ray, t_world *world, int depth);
void			ft_swap(float *a, float *b);

// parser.c
int				*resolution(char **data, t_world *world);
void			figures_parser(char **line, t_world *world);
void			scene_parser(char **line, t_world *world);
int				parser_file(int fd, t_world *world);

// elements.c
t_camera		*new_camera(char **data);
t_light			*new_light(char **data, t_world *world);
t_light			*new_ambient_light(char **data, t_world *world);
void			compute_sample_colors(t_thread_data *data, int i, int j,
					t_color pixel_colors[SAMPLES_PER_PIXEL]);

// vector.c
t_vector		vector(double x, double y, double z);
t_point			point(double x, double y, double z);

// free.c
void			free_world(t_world *world);
void			free_server(t_server *server);

// plane.c
t_plane			*new_plane(char **data);
int				hit_plane(t_ray *ray, t_plane *plane);

// square.c
void			square_vertex(t_square *square);
t_square		*new_square(char **data);
int				hit_square(t_ray *ray, t_square *square);
int				is_inside(t_hit r, t_vector *v, int vertex);

// cylinder.c
t_cylinder		*new_cylinder(char **data);
void			cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2]);
void			valid_hit(int *hit, float *dist, float *root, float *d,
					float *t);
float			solve(t_ray r, t_cylinder cy, float *d, int hit[2]);
int				hit_cylinder(t_ray *ray, t_cylinder *cylinder);

// sphere.c
t_sphere		*new_sphere(char **data);
void			sphere_roots(t_ray ray, t_sphere sphere, float *root);
int				hit_sphere(t_ray *ray, t_sphere *sphere);
void			fill_glass_material(t_ray *ray);
void			fill_mate_material(t_ray *ray);

// triangle.c
t_triangle		*new_triangle(char **data);
int				hit_triangle(t_ray *ray, t_triangle *triangle);

//cone.c
t_cone *new_cone(char **data);
int hit_cone(t_ray *ray, t_cone *cone);
void solve_quadratic(t_cone cone, float *roots, int *root_count);

// server.c
t_image			*new_image(t_server *server);
t_server		*new_server(t_world *world);
void			my_put_pixel(t_server *server, int x, int y, t_color color);
void			apply_stuff(t_ray *ray, t_world *world, int depth);
// camera.c
void			init_cameras(t_list *camset, int width, int height);
void			change_camera(t_server *server, int step);
t_ray			generate_ray(t_camera *camera, float u, float v);
void			move_camera(t_server *server, int code);
void			move_camera_rotate(t_server *server, int code);
void			move_camera_position(t_server *server, int code);
void			recalculate_camera(t_camera *camera, int width, int height);
void			rotate_camera_horizontal(t_server *server, float angle);
void			rotate_camera_vertical(t_server *server, float angle);

// events.c
int				exit_hook(t_server *server);
int				expose_hook(t_server *server);
void			update_hook(int keycode, t_server *server);
int				key_press_hook(int keycode, t_server *server);
int				mlx_events(t_server *server);
int				mouse_handler(int button, int x, int y, t_server *server);
void			update_ambient_color(t_server *server, int code);
void			update_texture(t_server *server);
void			update_material(t_server *server);
void			update_checkerboard(t_server *server);
void			update_phong(t_server *server);
void			update_bump(t_server *server);
void			create_filename(char **number, int *screenshot_counter,
					char **filename);
void			set_info(t_server *server, unsigned char bmp_info_header[40]);
void			set_file(int file_size, unsigned char bmp_file_header[14]);
void			set_header_info(t_server *server, int fd);
void			update_ambient_brightness(t_server *server, float num);
void			color_selec(t_server *server, char *pixels, int *var, int fd);
void			put_pixels(t_server *server, char *pixels, int fd);
void			take_screenshot(t_server *server);
// color.c
t_color			ccheck(int color);
t_color			cproduct(int color_a, int color_b);
t_color			cadd(int color_a, int color_b);
t_color			cscale(int color, float a);
int				in_shadow(t_light light, t_list *figures, t_hit record);
t_color			color_component(t_light light, t_hit record);
float			light_intensity(t_light light, t_hit record);

// render.c
int				intersec(t_ray *ray, t_list *figures);
t_color			average_color(t_color *colors, int num_colors);
void			*render_section(void *threadarg);
void			render(t_server *server);
void			render_low(t_server *server);

// render1.c
t_color			checkerboard_square(t_ray *ray);
t_color			checkerboard_sphere(t_ray *ray, float size);
t_color			checkerboard_cylinder(t_ray *ray);
t_color			checkerboard_pattern_selector(t_ray *ray);
void			treat_material(t_ray *ray, t_world *world, int depth);

// render2.c
t_color			get_texture_color(t_texture *texture, float u, float v);
t_color			phong_lighting(t_light light, t_ray *ray);
float			get_height_from_texture(t_texture *texture, float u, float v);
t_vector		calculate_tangent(t_vector normal);
t_vector		bump_function(t_hit record, t_texture *texture, float u,
					float v);

// render3.c
t_vector		apply_bump(t_hit record, t_world *world, float u, float v);
void			apply_texture_sphere(t_ray *ray, t_world *world, float *u,
					float *v);
void			apply_texture_square(t_ray *ray, t_world *world, float *u,
					float *v);
void			apply_texture_plane(t_ray *ray, t_world *world, float *u,
					float *v);
void			apply_texture_cylinder(t_ray *ray, t_world *world, float *u,
					float *v);

// render4.c
void			apply_texture(t_ray *ray, t_world *world);
void			apply_material(t_ray *ray, t_world *world, int depth);
void			apply_checkerboard(t_ray *ray, t_world *world);
int				check_depth_intersec(t_ray *ray, t_world *world, int depth);
t_color			raytracer(t_ray *ray, t_world *world, int depth);

// render5.c
void			render_pixel(t_thread_data *data, int i, int j);
void			sample_pixel_colors(t_thread_data *data, t_color *pixel_colors,
					int i, int j);
int				sample_pixel_row(t_thread_data *data, t_color *pixel_colors,
					int i, int j, int m, int k);
int				sample_pixel(t_thread_data *data, t_color *pixel_colors, int i,
					int j, int m, int n, int k);
float			calculate_u(int width, int i, int m);

// render6.c
float			calculate_v(int height, int j, int n);
void			initialize_colors(t_color pixel_colors[SAMPLES_PER_PIXEL]);
t_color			initialize_color(void);
void			show_menu(t_server *server);
void			render_selector(int t, t_thread_data *thread_data,
					t_server *server);
void			check_pthread_create(int rc, t_server *server);

// render7.c
void			fill_pixels(t_server *server, int i, int j,
					t_color pixel_color);
int				adjust_scale_factor(t_server *server);
void			*render_section_super(void *threadarg);
void			render_row(t_thread_data *data, int j);
void			join_menu_image(pthread_t *threads, t_server *server);

//utils_parse.c
int 				check_r(char **data);
int 				check_a(char **data);
int					check_c(char **data);
int					check_l(char **data);
int					check_pl(char **data);
int					check_sp(char **data);
int					check_sq(char **data);
int					check_cy(char **data);

//pre_parse.c
int pre_parse(char *file);

//utils_error.c
char **create_error(void);
void msg_exit(ErrorType error);

#endif
