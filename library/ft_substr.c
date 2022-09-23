
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 12:04:46 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/16 10:41:55 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"

static char	*ft_strcpy(char *s, char *str, unsigned int start, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	total;
	char	*str;

	if (!s)
		return (NULL);
	i = 0;
	total = ft_strlen(s) - start;
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (start + len > ft_strlen(s))
	{
		str = malloc((total + 1) * sizeof(char));
		if (!str)
			return (NULL);
		return (ft_strcpy(s, str, start, total));
	}
	else
	{
		str = malloc((len + 1) * sizeof(char));
		if (!str)
			return (NULL);
		return (ft_strcpy(s, str, start, len));
	}
}