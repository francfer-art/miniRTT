/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_abs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 20:00:28 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/03/09 20:00:35 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double	ft_abs(double n)
{
	if (n < 0)
		return (-1 * n);
	else
		return (n);
}
