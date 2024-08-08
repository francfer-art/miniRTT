/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:00:16 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/03/11 13:26:48 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

double	ft_atod(char *str)
{
	double	result;
	double	pow;
	int		sign;

	result = 0.0;
	pow = 0.1;
	sign = 1;
	while (*str == ' ')
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		result = (result * 10) + (*str++ - '0') + 0.0;
	if (*str++ == '.')
	{
		while (*str >= '0' && *str <= '9')
		{
			result += (*str++ - '0') * pow;
			pow *= 0.1;
		}
	}
	return (result * sign);
}
