/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 11:23:16 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/03/09 19:58:17 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		len_s;
	char	*str;

	str = (char *)s;
	len_s = ft_strlen(str);
	while (len_s >= 0)
	{
		if (str[len_s] == (unsigned char)c)
			return (&str[len_s]);
		len_s--;
	}
	return (0);
}
