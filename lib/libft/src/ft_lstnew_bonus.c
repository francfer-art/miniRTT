/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francfer <francfer@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 19:57:03 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/08/28 17:21:20 by francfer         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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
	n_lst->prev = NULL;
	n_lst->next = NULL;
	return (n_lst);
}
