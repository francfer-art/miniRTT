/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:45:36 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/05/13 11:02:30 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	word_count(char const *s, char c)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
		{
			i++;
		}
		if (s[i] != c && s[i])
		{
			j++;
		}
		while (s[i] != c && s[i])
		{
			i++;
		}
	}
	return (j);
}

static int	str_len(char const *s, int c, int i)
{
	int	len;

	len = 0;
	while (s[i] && s[i] != c)
	{
		len++;
		i++;
	}
	return (len);
}

static char	**free_all(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i++]);
	}
	free(s);
	return (NULL);
}

static int	skip_prefix(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] == c && s[i])
	{
		i++;
	}
	return (i);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	int		start;
	int		len;
	int		i;
	int		array_num;

	array_num = -1;
	i = 0;
	array = (char **)malloc((word_count(s, c) + 1) * sizeof(char *));
	if (!s || array == 0)
		return (0);
	while (s[i])
	{
		len = 0;
		i += skip_prefix(&s[i], c);
		start = i;
		len = str_len(s, c, i);
		i += len;
		if (len)
			array[++array_num] = ft_substr(s, start, len);
		if (len && !array[array_num])
			return (free_all(array));
	}
	array[++array_num] = NULL;
	return (array);
}
