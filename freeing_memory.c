/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_memory.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 19:12:55 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 15:45:23 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "library.h"

t_freeing_memory	*ft_lstnew(void *content)
{
	t_freeing_memory	*list;

	list = malloc(sizeof(t_freeing_memory));
	if (!list)
		return (NULL);
	list->content = content;
	list->next = NULL;
	return (list);
}

void	*alocate(t_freeing_memory **head, int size)
{
	void	*data;

	data = malloc(size);
	if (!data)
		return (NULL);
	ft_lstadd_back(head, ft_lstnew(data));
	return (data);
}
