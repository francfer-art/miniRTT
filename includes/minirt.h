#ifndef MINIRT_H
# define MINIRT_H

# include <mlx.h>
# include <fcntl.h>
# include <libft.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <X11/X.h>
# include <X11/keysym.h>

// Extension archivo
# define EXTENSION ".rt"

// Nombre ventana
# define WINDOW_TITLE "miniRT"

// Variable para evitar errores
# define EPSILON 0.0001

//MATERIALES
# define ALBEDO 1000
# define ALBEDO_POLISHED_METAL 900
# define ALBEDO_GLASS 700
# define ALBEDO_PLASTIC 500
# define ALBEDO_MATTE 300
# define ALBEDO_NONE 0

// Definición de errores 
# define ERROR_MALLOC "ERROR : Malloc problem 🔴"
# define ERROR_PARSE "ERROR : While parsing 🔴"
# define ERROR_COLOR "ERROR : Invalid color definition 🔴"
# define ERROR_SERVER "ERROR : Can't create server 🔴"
# define ERROR_COL_VAL "ERROR : Color value out range 🔴"
# define ERROR_VECTOR "ERROR : Vector out range 🔴"
# define ERROR_BRIGHTNESS "ERROR : Brightness out of range 🔴"
# define ERROR_RATIO "ERROR : Ambient light ratio out of range 🔴"
# define ERROR_DIR "ERROR : Invalid number detected 🔴"
# define ERROR_ARG "ERROR : Incorrect number of arguments 🔴"
# define ERROR_ID "ERROR : Invalid object's identifier 🔴"
# define ERROR_RES "ERROR : Resolution not found 🔴"
# define ERROR_RES_LEN "ERROR : Poor resolution definition 🔴"
# define ERROR_RES_VALUE "ERROR : Resolution value below 0 🔴"
# define ERROR_AMB "ERROR : Not ambient light found 🔴"
# define ERROR_FORMAT "ERROR : File format not supported"
# define ERROR_OPEN "ERROR : Error opening the file 🔴"
# define ERROR_CLOSE "ERROR : Error closing the file 🔴"
# define ERROR_MEMORY "ERROR : Memory error"
# define ERROR_LINE "ERROR : Line length discrepancy"
# define ERROR_LINES "ERROR : Not enought lines"
# define ERROR_READ "ERROR : Error reading file"

typedef int			t_color;

typedef enum s_type
{
	SPHERE,
	PLANE,
	SQUARE,
	TRIANGLE,
	CYLINDER
}					t_type;

typedef struct s_point
{
	double			x;
	double			y;
	double			z;
}					t_point;

typedef struct s_vector
{
	double			x;
	double			y;
	double			z;
}					t_vector;

typedef struct s_sphere
{
	t_type			type;
	t_vector		center;
	float			radius;
	t_color			color;
}					t_sphere;

typedef struct s_plane
{
	t_type			type;
	t_vector		point;
	t_vector		normal;
	t_color			color;
}					t_plane;

typedef struct s_quare
{
	t_type			type;
	t_vector		center;
	float			side;
	t_vector		normal;
	t_vector		vertex[4];
	t_color			color;
}					t_square;

typedef struct s_cylinder
{
	t_type			type;
	t_vector			center;
	t_vector		axis;
	float			radius;
	float			height;
	t_vector		edege[2];
	t_color			color;
}					t_cylinder;

typedef struct s_triangle
{
	t_type			type;
	t_vector		vertex[3];
	t_vector		normal;
	t_color			color;
}					t_triangle;

typedef struct s_camera
{
	t_vector		origin;
	t_vector		direction;
	t_vector		horizontal;
	t_vector		vertical;
	t_vector		llc;	
	float			fov;
}					t_camera;

typedef struct s_light
{
	t_vector		position;
	float			brightness;
	t_color			color;
}					t_light;

typedef struct s_hit
{
	t_vector		normal;
	t_vector		p;
	float			t;
	void			*object;
	t_color			color;
	t_type			type;
}					t_hit;

typedef struct s_ray
{
	t_vector		origin;
	t_vector		direction;
	t_hit			record;
}					t_ray;

typedef struct s_world
{
	int				*resolution;
	t_list			*figures;
	t_list			*lights;
	t_list			*cameras;
	t_light			*ambient;
}					t_world;

typedef struct s_image
{
	void			*image;
	char			*data;
	int				bpp;
	int				endian;
	int				size_line;
}					t_image;

typedef struct s_server
{
	void			*mlx;
	void			*window;
	t_image			*image;
	t_world			*world;
	unsigned int	width;
	unsigned int	height;
}					t_server;


//error.c
void				message_exit(char *msg);
void				full_message_exit(char *msg, t_world *world, t_server *server);

//scene.c
t_world				*scene_init(char *file);
int					open_scene_file(char *file);
t_world				*new_world(void);

//utils.c
int					double_pointer_len(char **data);
int					out_range_vector(t_vector v);
float				degree_2_rad(float degree);
float				ft_atof(char *str);
t_vector			ft_atov(char *str);

//utils1.c
void				free_double(char **s);
int					invalid_number(char *str);
t_color				ft_atoc(char *str);
t_vector			norm(t_vector v);
t_vector			scale(t_vector v, double c);

//utils2.c
double				sqr(double num);
double				length_square(t_vector v);
double				length(t_vector v);
double				dot(t_vector v, t_vector w);
t_vector			add(t_vector v, t_vector w);
t_vector			sub(t_vector v, t_vector w);
t_vector			at(t_ray ray);
t_vector			cross(t_vector v, t_vector w);

//parser.c
int					*resolution(char **data);
void				figures_parser(char **line, t_world *world);
void				scene_parser(char **line, t_world *world);
int					parser_file(int fd, t_world *world);

//elements.c
t_camera			*new_camera(char **data);
t_light				*new_light(char **data);
t_light				*new_ambient_light(char **data);

//vector.c
t_vector			vector(double x, double y, double z);
t_point				point(double x, double y, double z);

//free.c
void				free_world(t_world *world);
void				free_server(t_server *server);

//plane.c
t_plane				*new_plane(char **data);
int					hit_plane(t_ray *ray, t_plane *plane);

//square.c
void    			square_vertex(t_square *square);
t_square    		*new_square(char **data);
int					hit_square(t_ray *ray, t_square *square);
int					is_inside(t_hit r, t_vector *v, int vertex);

//cylinder.c
t_cylinder			*new_cylinder(char **data);
void				cylinder_roots(t_ray r, t_cylinder cylinder, float roots[2]);
void				valid_hit(int *hit, float *dist, float *root, float *d, float *t);
float				solve(t_ray r, t_cylinder cy, float *d, int hit[2]);
int					hit_cylinder(t_ray *ray, t_cylinder *cylinder);

//sphere.c
t_sphere			*new_sphere(char **data);
void				sphere_roots(t_ray ray, t_sphere sphere, float *root);
int					hit_sphere(t_ray *ray, t_sphere *sphere);

//triangle.c
t_triangle			*new_triangle(char **data);
int					hit_triangle(t_ray *ray, t_triangle *triangle);

//server.c
t_image				*new_image(t_server *server);
t_server			*new_server(t_world *world);
void				my_put_pixel(t_server *server, int x, int y, t_color color);

//camera.c
void    			init_cameras(t_list *camset, int width, int height);
void    			change_camera(t_server *server, int step);
t_ray   			generate_ray(t_camera *camera, float u, float v);
void				move_camera(t_server *server, int code);

//events.c
int 				exit_hook(t_server *server);
int					expose_hook(t_server *server);
int 				key_press_hook(int keycode, t_server *server);
void    			mlx_events(t_server *server);

//color.c
t_color				ccheck(int color);
t_color 			cproduct(int color_a, int color_b);
t_color 			cadd(int color_a, int color_b);
t_color 			cscale(int color, float a);
int					in_shadow(t_light light, t_list *figures, t_hit record);
t_color				color_component(t_light *light, t_hit record);
float				light_intensity(t_light light, t_hit record);

//render.c
int 				intersec(t_ray *ray, t_list *figures);
t_color 			raytracer(t_ray *ray, t_world *world);
void				render(t_server *server);

#endif