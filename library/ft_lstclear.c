
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:37:12 by ael-hiou          #+#    #+#             */
/*   Updated: 2021/11/15 21:28:41 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"
void	ft_lstclear(t_freeing_memory **lst)
{
	t_freeing_memory	*temp;
	t_freeing_memory	*saver;

	temp = *lst;
	while (temp != NULL)
	{
		saver = temp ->next;
		if (temp && temp->address)
			free (temp->address);
		if (temp)
			free(temp);
		temp = saver;
	}
	*lst = NULL;
}
