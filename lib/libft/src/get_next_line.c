/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 09:00:30 by vsanz-su          #+#    #+#             */
/*   Updated: 2024/03/09 19:57:50 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static char	*ft_get_line(char *line)
{
	int		i;
	char	*s;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (line[i] == '\0')
	{
		return (NULL);
	}
	s = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (!s)
	{
		return (NULL);
	}
	if (s[0] == '\0')
	{
		free(s);
		s = NULL;
		return (s);
	}
	line[i + 1] = '\0';
	return (s);
}

static char	*ft_read_save(int fd, char *buf, char *stack)
{
	char	*tmp;
	int		bytes_read;

	bytes_read = 1;
	while (bytes_read != 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read == -1)
			return (NULL);
		else if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		if (!stack)
			stack = ft_strdup("");
		tmp = stack;
		stack = ft_strjoin(tmp, buf);
		if (!stack)
			return (NULL);
		free(tmp);
		tmp = NULL;
		if (ft_strchr(buf, '\n'))
			break ;
	}
	return (stack);
}

char	*get_next_line(int fd)
{
	static char	*stack;
	char		*buf;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buf)
		return (NULL);
	line = ft_read_save(fd, buf, stack);
	free(buf);
	if (!line)
	{
		free(stack);
		stack = NULL;
		return (NULL);
	}
	stack = ft_get_line(line);
	return (line);
}
