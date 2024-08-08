#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	char	*orig_dest;

	orig_dest = dest;
	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return (orig_dest);
}
