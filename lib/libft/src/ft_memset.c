/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:21:40 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/10/05 18:27:28 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*byte_b;
	unsigned char	byte_value;
	size_t			i;

	byte_b = (unsigned char *)b;
	byte_value = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		byte_b[i] = byte_value;
		i++;
	}
	return (b);
}
