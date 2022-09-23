
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 12:20:15 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/26 16:07:58 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *src)
{
	size_t	n;
	size_t	i;
	char	*p;

	if (!src)
		return (NULL);
	n = ft_strlen(src);
	p = malloc(sizeof(char) * (n + 1));
	if (p)
	{
		i = 0;
		while (src[i])
		{
			p[i] = src[i];
			i++;
		}
		p[i] = '\0';
	}
	else
		return (0);
	return (p);
}

void	ft_strlcpy(char *dst, char *src, int dstsize)
{
	int	i;
	int	size;

	size = dstsize;
	i = 0;
	if (dstsize > 0)
	{
		while (i < size - 1 && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	size_t	s1_len;
	size_t	s2_len;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	i = -1;
	if (!s1)
		s1_len = 0;
	else
		s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	j = 0;
	str = malloc((s1_len + s2_len + 1) * sizeof(char));
	while (++i < s1_len)
		str[i] = s1[i];
	while (j < s2_len)
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	return (str);
}