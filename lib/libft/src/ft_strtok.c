#include <libft.h>

char	*ft_strtok(char *str, const char *delim)
{
	static char	*ptr = NULL;
	char		*start;
	char		*end;

	if (str != NULL)
		ptr = str;
	else if (ptr == NULL)
		return (NULL);
	start = ptr;
	while (*start != '\0' && ft_strchr(delim, *start) != NULL)
		start++;
	if (*start == '\0')
		return (ptr = NULL, NULL);
	end = start;
	while (*end != '\0' && ft_strchr(delim, *end) == NULL)
		end++;
	if (*end != '\0')
	{
		*end = '\0';
		ptr = end + 1;
	}
	else
		ptr = NULL;
	return (start);
}
