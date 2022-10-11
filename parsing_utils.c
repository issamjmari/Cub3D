/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 11:15:12 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	counter_checker(int *texturecounter, char *message)
{
	if (*texturecounter == 1)
		error_message(message);
	*texturecounter = 1;
}

void	ft_free(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		free(data[i++]);
	free (data);
}

void	error_message(char *message)
{
	printf("%s\n", message);
	exit (1);
}

int	get_size(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	return (i);
}

void	missing_texture(t_checkDuplicate *vars)
{
	if (vars->socounter == 0)
		error_message(SOUTH_TEXTURE_MISSING);
	else if (vars->wecounter == 0)
		error_message(WEST_TEXTURE_MISSING);
	else if (vars->nocounter == 0)
		error_message(NORTH_TEXTURE_MISSING);
	else if (vars->eacounter == 0)
		error_message(EAST_TEXTURE_MISSING);
	else if (vars->fcounter == 0)
		error_message(FLOOR_COLOR_MISSING);
	else if (vars->ccounter == 0)
		error_message(CEILING_COLOR_MISSING);
}
