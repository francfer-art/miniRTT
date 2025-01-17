#include <minirt.h>

void	message_exit(t_error msg)
{
	if (msg)
	{
		printf("Unexpected error!\n");
	}
	exit(EXIT_FAILURE);
}

void	full_message_exit(t_error msg, t_world *world, t_server *server)
{
	free_world(world);
	free_server(server);
	msg_exit(msg);
}

void	set_texture(t_world *world)
{
	world->texture_img = malloc(sizeof(t_texture));
	if (!world->texture_img)
	{
		full_message_exit(ERROR_ARG, world, NULL);
	}
	world->texture_img->img_ptr = NULL;
	world->texture_img->img_data = NULL;
	world->texture_img->width = 0;
	world->texture_img->height = 0;
	world->texture_img->bpp = 0;
	world->texture_img->size_line = 0;
	world->texture_img->endian = 0;
}
