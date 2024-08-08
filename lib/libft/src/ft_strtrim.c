/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:38:54 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/10/03 12:51:51 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int	size;

	if (s1 == 0 || set == 0)
	{
		return (0);
	}
	while (ft_strchr(set, *s1) && *s1 != '\0')
	{
		s1++;
	}
	size = ft_strlen((char *)s1);
	while (ft_strchr(set, s1[size]) && size > 0)
	{
		size--;
	}
	return (ft_substr((char *)s1, 0, size + 1));
}
