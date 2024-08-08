/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:21:16 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/09/21 11:21:19 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	if (dest == src)
		return (dest);
	if (!n)
		return (dest);
	i = 0;
	d = (char *)dest;
	s = (char *)src;
	while (i < n)
	{
		if (d || s)
			d[i] = s[i];
		i++;
	}
	return ((void *)dest);
}
