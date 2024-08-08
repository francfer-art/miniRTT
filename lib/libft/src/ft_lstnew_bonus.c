/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:57:03 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/03/09 19:57:05 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*n_lst;

	n_lst = (t_list *)malloc(sizeof(t_list));
	if (!n_lst)
	{
		return (NULL);
	}
	n_lst->content = content;
	n_lst->next = NULL;
	return (n_lst);
}
