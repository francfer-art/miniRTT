/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:23:06 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/09/21 11:23:08 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	size_to_find;
	char	*str;
	char	*to_find;

	str = (char *)haystack;
	to_find = (char *)needle;
	size_to_find = ft_strlen(needle);
	if (size_to_find == 0)
	{
		return (str);
	}
	while (*str && len-- >= size_to_find)
	{
		if (*str == *to_find && ft_memcmp(str, to_find, size_to_find) == 0)
		{
			return (str);
		}
		str++;
	}
	return (NULL);
}
