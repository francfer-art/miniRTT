/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 13:42:47 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/09/21 16:46:07 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*d;
	int		n;

	n = 0;
	while (s1[n])
	{
		n++;
	}
	d = (char *)malloc((n + 1) * sizeof(char));
	if (!d)
	{
		return (NULL);
	}
	n = 0;
	while (s1[n])
	{
		d[n] = s1[n];
		n++;
	}
	d[n] = '\0';
	return (d);
}
