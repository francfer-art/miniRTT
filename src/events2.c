#include <minirt.h>

void	update_bump(t_server *server)
{
	if (!server->world->bump && server->world->texture)
		server->world->bump = 1;
	else
		server->world->bump = 0;
}

void	create_filename(char **number, int *screenshot_counter, char **filename)
{
	*filename = malloc(sizeof(char *));
	if (!*filename)
		return ;
	*number = ft_itoa(*screenshot_counter);
	ft_strcpy(*filename, "save/screenshot_");
	ft_strcat(*filename, *number);
	ft_strcat(*filename, ".bmp");
	free(*number);
	(*screenshot_counter)++;
}

void	set_info(t_server *server, unsigned char bmp_info_header[40])
{
	bmp_info_header[0] = 40;
	bmp_info_header[1] = 0;
	bmp_info_header[2] = 0;
	bmp_info_header[3] = 0;
	bmp_info_header[4] = (unsigned char)(server->width);
	bmp_info_header[5] = (unsigned char)(server->width >> 8);
	bmp_info_header[6] = (unsigned char)(server->width >> 16);
	bmp_info_header[7] = (unsigned char)(server->width >> 24);
	bmp_info_header[8] = (unsigned char)(server->height);
	bmp_info_header[9] = (unsigned char)(server->height >> 8);
	bmp_info_header[10] = (unsigned char)(server->height >> 16);
	bmp_info_header[11] = (unsigned char)(server->height >> 24);
	bmp_info_header[12] = 1;
	bmp_info_header[13] = 0;
	bmp_info_header[14] = 24;
	bmp_info_header[15] = 0;
}

void	set_file(int file_size, unsigned char bmp_file_header[14])
{
	bmp_file_header[0] = 'B';
	bmp_file_header[1] = 'M';
	bmp_file_header[2] = (unsigned char)(file_size);
	bmp_file_header[3] = (unsigned char)(file_size >> 8);
	bmp_file_header[4] = (unsigned char)(file_size >> 16);
	bmp_file_header[5] = (unsigned char)(file_size >> 24);
	bmp_file_header[6] = 0;
	bmp_file_header[7] = 0;
	bmp_file_header[8] = 0;
	bmp_file_header[9] = 0;
	bmp_file_header[10] = 54;
	bmp_file_header[11] = 0;
	bmp_file_header[12] = 0;
	bmp_file_header[13] = 0;
}

void	set_header_info(t_server *server, int fd)
{
	int				file_size;
	unsigned char	*bmp_file_header;
	unsigned char	*bmp_info_header;

	bmp_file_header = malloc(14);
	if (!bmp_file_header)
		return ;
	bmp_info_header = malloc(40);
	if (!bmp_info_header)
		return ;
	file_size = 454 + (3 * server->width * server->height);
	set_file(file_size, bmp_file_header);
	set_info(server, bmp_info_header);
	write(fd, bmp_file_header, 14);
	write(fd, bmp_info_header, 40);
	free(bmp_file_header);
	free(bmp_info_header);
}
