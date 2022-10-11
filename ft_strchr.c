/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 12:30:44 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 15:46:52 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"

char	*search(char *s, int c)
{
	int	a;

	if (!s)
		return (NULL);
	a = (int)ft_strlen(s) - 1;
	if (c == 0)
		return ((char *)&s[a + 1]);
	while (a >= 0)
	{
		if (s[a] == (char)c)
			return ((char *)&s[a]);
		a--;
	}
	return (NULL);
}
