#include <minirt.h>

char	**create_error(void)
{
	char	**error_messages;

	error_messages = malloc(ERROR_COUNT * sizeof(char *));
	if (error_messages == NULL)
		return (NULL);
	error_messages[ERROR_JEFE] = strdup("🐷 Nos vemos jefe 🐷");
	error_messages[ERROR_PARSE] = strdup("ERROR: While parsing 🔴");
	error_messages[ERROR_CREATE] = strdup("ERROR: return (pthread_create() 🔴");
	error_messages[ERROR_JOIN] = strdup("ERROR: return (pthread_join() 🔴");
	error_messages[ERROR_COLOR] = strdup("ERROR: Invalid color definition 🔴");
	error_messages[ERROR_BRIGHTNESS] = strdup("ERROR: Bright out of range 🔴");
	error_messages[ERROR_DIR] = strdup("ERROR: Invalid number detected 🔴");
	error_messages[ERROR_ARG] = strdup("ERROR: Bad number of arguments 🔴");
	error_messages[ERROR_ID] = strdup("ERROR: Invalid object's identifier 🔴");
	error_messages[ERROR_RES] = strdup("ERROR: Resolution not found 🔴");
	error_messages[ERROR_RES_LEN] = strdup("ERROR: Resolution 🔴");
	error_messages[ERROR_RES_VALUE] = strdup("ERROR: Resolution below 0 🔴");
	error_messages[ERROR_AMB] = strdup("ERROR: Not ambient light found 🔴");
	error_messages[ERROR_FORMAT] = strdup("ERROR: File format not supported 🔴");
	error_messages[ERROR_OPEN] = strdup("ERROR: Error opening the file 🔴");
	error_messages[ERROR_CLOSE] = strdup("ERROR: Error closing the file 🔴");
	error_messages[ERROR_CAMERA] = strdup("ERROR: Invalid camera definition 🔴");
	return (error_messages);
}

void	msg_exit(ErrorType error)
{
	char	**error_messages;
	int		i;

	i = 0;
	error_messages = create_error();
	if (error != 0 && error < ERROR_COUNT)
	{
		ft_putendl_fd(error_messages[error], STDERR_FILENO);
		while (i < ERROR_COUNT)
			free(error_messages[i++]);
		free(error_messages);
		exit(EXIT_FAILURE);
	}
	else if (error == 0)
	{
		ft_printf(error_messages[error]);
		while (i < ERROR_COUNT)
			free(error_messages[i++]);
		free(error_messages);
		exit(EXIT_SUCCESS);
	}
}
