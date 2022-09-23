
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/06 09:25:51 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/16 09:49:41 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"

static char	*ft_alocateandcopy(char *s, int length)
{
	char	*p;

	p = malloc(sizeof(char) * length);
	if (!p)
		return (NULL);
	ft_strlcpy(p, s, length);
	return (p);
}

static int	ft_counting(char *s, char c)
{
	size_t	i;
	int		cp;

	i = 1;
	cp = 0;
	if (s[0] != c)
		cp++;
	while (s[i])
	{
		if (s[i] != c && s[i - 1] == c)
			cp++;
		i++;
	}
	return (cp);
}

static void	ft_next(char **str, char *s, char c, int size)
{
	int	i;
	int	j;
	int	st;

	i = 0;
	st = 0;
	j = 0;
	while (i < size)
	{
		while (s[j] && s[j] == c)
			j++;
		st = j;
		while (s[j] && s[j] != c)
			j++;
		str[i] = ft_alocateandcopy(s + st, j - st + 1);
		if (str[i] == NULL)
		{
			while (i >= 0)
				free(str[i--]);
			free(str);
			return ;
		}
		i++;
	}
	str[i] = 0;
}

char	**ft_split(char *s, char c)
{
	char	**str;
	int		size;

	if (!s)
		return (NULL);
	if ((!ft_strlen(s) && !c) || !ft_strlen(s))
	{
		str = malloc (sizeof(char *));
		str[0] = NULL;
		return (str);
	}
	size = ft_counting(s, c);
	str = (char **)malloc(sizeof(char *) * (size + 1));
	if (!str)
		return (NULL);
	ft_next(str, s, c, size);
	return (str);
}