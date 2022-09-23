
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 10:40:28 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/26 16:20:58 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strchr(char *s, int c)
{
	int		i;

	i = 0;
	c = (char)c;
	if (!s)
		return (-1);
	while (s[i] && s[i] != c)
		i++;
	if (s[i] == c)
		return (i);
	return (-1);
}

char	*coppy_remainder(char *str, int index)
{
	int		k;
	int		i;
	int		len;	
	char	*remainder;

	i = index;
	len = ft_strlen(str + index);
	k = 0;
	remainder = malloc(sizeof(char) * (len + 1));
	while (str[i])
		remainder[k++] = str[i++];
	remainder[k] = '\0';
	free(str);
	return (remainder);
}

char	*ft_cut(char *str, int index)
{
	char	*line;

	line = malloc(sizeof(char) * (index + 1));
	ft_strlcpy(line, str, index + 1);
	return (line);
}

char	*ft_helper(int fd, char *buffer)
{
	int		k;
	char	*temp;

	k = 1;
	temp = malloc (sizeof(char) * (BUFFER_SIZE + 1));
	while ((ft_strchr(buffer, '\n') == -1) && k != 0)
	{
		k = read(fd, temp, BUFFER_SIZE);
		if (k == -1)
		{
			free(temp);
			return (NULL);
		}
		temp[k] = '\0';
		buffer = ft_strjoin(buffer, temp);
	}
	free(temp);
	if (k == 0 && ft_strlen(buffer) == 0)
	{
		free(buffer);
		buffer = NULL;
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer = NULL;
	int			index_new_line;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (ft_strchr(buffer, '\n') == -1)
		buffer = ft_helper(fd, buffer);
	if (!buffer)
		return (NULL);
	index_new_line = ft_strchr(buffer, '\n');
	if (index_new_line == -1)
	{
		line = buffer;
		buffer = NULL;
		return (line);
	}
	line = ft_cut(buffer, index_new_line + 1);
	buffer = coppy_remainder(buffer, index_new_line + 1);
	return (line);
}
