/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 11:15:12 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/10 14:42:14 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void    texturescounterChecker(int *texturecounter, char *message)
{
	if (*texturecounter == 1)
		errorMessage(message);
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

void	errorMessage(char *message)
{
	printf("%s\n", message);
	exit (1);
}

int	getSize(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	return (i);
}

void    missingTexture(t_checkDuplicate *vars)
{
    if (vars->socounter == 0)
        errorMessage(south_TEXTURE_MISSING);
    else if (vars->wecounter == 0)
        errorMessage(west_TEXTURE_MISSING);
    else if (vars->nocounter == 0)
        errorMessage(north_TEXTURE_MISSING);
    else if (vars->eacounter == 0)
        errorMessage(east_TEXTURE_MISSING);
    else if (vars->fcounter == 0)
        errorMessage(floor_color_MISSING);
    else if (vars->ccounter == 0)
        errorMessage(ceiling_color_MISSING);
}
