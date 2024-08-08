#include <minirt.h>

void	message_exit(char *msg)
{
	if (msg)
	{
		ft_putstr_fd(msg, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	exit(EXIT_FAILURE);
}
