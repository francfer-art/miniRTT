/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 08:47:42 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/11/03 17:28:02 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static void	ft_format(va_list args, const char format, int *n_chr)
{
	unsigned long	p_aux;

	if (format == 'c')
		ft_putchar(va_arg(args, int), n_chr);
	else if (format == 's')
		ft_putstr(va_arg(args, char *), n_chr);
	else if (format == 'p')
	{
		ft_putstr("0x", n_chr);
		p_aux = va_arg(args, unsigned long long);
		if (p_aux == 0)
			ft_putchar('0', n_chr);
		else
			ft_put_nbr_base("0123456789abcdef", p_aux, n_chr);
	}
	else if (format == 'd' || format == 'i')
		ft_put_nbr_base("0123456789", va_arg(args, int), n_chr);
	else if (format == 'u')
		ft_put_nbr_base("0123456789", va_arg(args, unsigned int), n_chr);
	else if (format == 'x')
		ft_put_nbr_base("0123456789abcdef", va_arg(args, unsigned int), n_chr);
	else if (format == 'X')
		ft_put_nbr_base("0123456789ABCDEF", va_arg(args, unsigned int), n_chr);
	else if (format == '%')
		ft_putchar('%', n_chr);
}

int	ft_printf(const char *str, ...)
{
	int		i;
	va_list	args;
	int		n_chr;

	i = 0;
	n_chr = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			ft_format(args, str[i + 1], &n_chr);
			i++;
		}
		else
			ft_putchar(str[i], &n_chr);
		i++;
	}
	va_end(args);
	return (n_chr);
}
