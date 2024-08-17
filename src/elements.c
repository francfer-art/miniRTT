#include <minirt.h>

// Función que crea un puntero a una estructura t_camera desde un char **
// Lo primero que comprobamos es la longitud del char **, si es distinto de 4
// devolvemos NULL. Si la longitud es correcta alojamos la memoria y rellenamos
// la estructura
// Si el fov es negativo o mayor a 180 liberamos la cámara y devolvemos NULL.
// Finalmente devolvemos el puntero a la estructura
// PD: La dirección de la cámara se normaliza de base, para asegurarnos de 
// que el vector no está fuera de rango
t_camera	*new_camera(char **data)
{
	t_camera	*camera;
	float		fov;

	if (double_pointer_len(data) != 4)
		return (NULL);
	camera = malloc(sizeof(*camera));
	if (!camera)
		return (NULL);
	fov = ft_atof(data[3]);
	if (fov < 0 || fov > 180)
		return (free(camera), NULL);
	camera->fov = degree_2_rad(fov);
	camera->direction = norm(ft_atov(data[2]));
	camera->origin = ft_atov(data[1]);
	return (camera);
}

// Función para crear un puntero a una  estructura t_light. Lo primero que hacemos
// es comprobar la longitud del char **. Si la longitud es distinta de 4 devolvemos
// NULL. Alojamos dinámicamente la memoria y procedemos a rellenar la estructura
// con el char **. Comprobamos que el brillo de la luz no sea menor que 0 o mayor
// que 1. Finalmente devolvemos la referencia a la estructura.
t_light	*new_light(char **data)
{
	t_light	*light;

	if (double_pointer_len(data) != 4)
		return (NULL);
	light = malloc(sizeof(*light));
	if (!light)
		return (NULL);
	light->color = ft_atoc(data[3]);
	light->brightness = ft_atof(data[2]);
	if (light->brightness < 0 || light->brightness > 1)
		message_exit(ERROR_BRIGHTNESS);
	light->position = ft_atov(data[1]);
	return (light);
}

// Función para crear un puntero a la estructura t_light. Lo primero
// que hacemos es comprobar que la longitud del char ** no sea distinta de 3.
// Si la longitud es correcta, alojamos dinámicamente la memoria y procedemos a
// rellenar la estructura. Si el brillo es negativo o mayor que 1 mostramos
// un error. Finalmente devolvemos la referencia a la estructura.
// Es muy similar a la estructura anterior, con la única diferencia que la luz
// ambiente no tiene posición.
t_light	*new_ambient_light(char **data)
{
	t_light	*light;

	if (double_pointer_len(data) != 3)
		return (NULL);
	light = malloc(sizeof(*light));
	if (!light)
		return (NULL);
	light->color = ft_atoc(data[2]);
	light->brightness = ft_atof(data[1]);
	if (light->brightness < 0 || light->brightness > 1)
		message_exit(ERROR_BRIGHTNESS);
	return (light);
}
