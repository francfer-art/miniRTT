#include <minirt.h>

int	main(int args, char **argv)
{
	int			status;
	t_world		*world;
	t_server	*x;

	if (args < 2 || args > 3)
		return (message_exit(ERROR_ARG));
	world = scene_init(argv[1]);
}
