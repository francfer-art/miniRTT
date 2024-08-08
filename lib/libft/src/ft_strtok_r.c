#include <libft.h>

char	*ft_strtok_r(char *str, const char *delim, char **save_ptr)
{
	char	*start;
	char	*end;

	if (str != NULL)
		*save_ptr = str;
	else if (*save_ptr == NULL || **save_ptr == '\0')
		return (NULL);
	start = *save_ptr;
	while (*start != '\0' && ft_strchr(delim, *start) != NULL)
		start++;
	if (*start == '\0')
		return (*save_ptr = NULL, NULL);
	end = start;
	while (*end != '\0' && ft_strchr(delim, *end) == NULL)
		end++;
	if (*end != '\0')
	{
		*end = '\0';
		*save_ptr = end + 1;
	}
	else
		*save_ptr = NULL;
	return (start);
}
