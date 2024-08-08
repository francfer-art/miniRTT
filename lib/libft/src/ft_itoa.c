/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 12:21:44 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/10/03 10:14:43 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_len(long long n)
{
	int	is_negative;
	int	i;

	is_negative = 0;
	i = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (is_negative + i);
}

char	*ft_itoa(int n)
{
	int			len;
	long long	aux;
	char		*p;

	len = num_len(n);
	aux = n;
	if (aux < 0)
		aux = -aux;
	p = (char *)malloc((len + 1) * sizeof(char));
	if (p == 0)
		return (0);
	p[len--] = '\0';
	if (n == 0)
	{
		p[0] = '0';
		return (p);
	}
	while (aux > 0)
	{
		p[len--] = (aux % 10) + '0';
		aux /= 10;
	}
	if (n < 0)
		p[0] = '-';
	return (p);
}
