

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 16:15:52 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/16 10:24:25 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"

int	ft_check(char *set, char a)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == a)
			return (1);
		i++;
	}
	return (0);
}

size_t	ft_right(char *s1, char *set)
{
	size_t	j;

	j = ft_strlen(s1) - 1;
	while (j >= 0)
	{
		if (!(ft_check(set, s1[j])))
			break ;
		j--;
	}
	return (j);
}

size_t	ft_left(char *s1, char *set)
{
	size_t	i;

	i = 0;
	while (s1[i])
	{
		if (!(ft_check(set, s1[i])))
			break ;
		i++;
	}
	return (i);
}

char	*ft_strtrim(char *s1, char *set)
{
	size_t	i;
	size_t	j;
	char	*str;
	size_t	k;

	if (!s1 || !set)
		return (NULL);
	k = 0;
	i = ft_left(s1, set);
	if (i == ft_strlen(s1))
		return (ft_strdup(""));
	j = ft_right(s1, set);
	str = malloc(sizeof(char) * j - i + 2);
	if (!str)
		return (NULL);
	while (i <= j)
		str[k++] = s1[i++];
	str[k] = '\0';
	return (str);
}