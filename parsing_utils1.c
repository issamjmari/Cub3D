/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 14:26:31 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	choosing_direction_utils(char *first_part, char *second_part, \
t_checkDuplicate *vars, t_directions *path)
{
	if (!ft_strncmp(first_part, N))
	{
		counter_checker(&vars->nocounter, NORTH_TEXTURE_DUPLICATE);
		free (first_part);
		path->north = second_part;
	}
	else if (!ft_strncmp(first_part, W))
	{
		counter_checker(&vars->wecounter, WEST_TEXTURE_DUPLICATE);
		free (first_part);
		path->west = second_part;
	}
	else if (!ft_strncmp(first_part, E))
	{
		counter_checker(&vars->eacounter, EAST_TEXTURE_DUPLICATE);
		free (first_part);
		path->east = second_part;
	}
	else if (!ft_strncmp(first_part, S))
	{
		counter_checker(&vars->socounter, SOUTH_TEXTURE_DUPLICATE);
		free (first_part);
		path->south = second_part;
	}
}

void	choosing_directions(char *first_part, char *second_part, \
t_directions *path, t_checkDuplicate *checkDuplicate)
{
	if (open(second_part, R_OK) == -1)
		error_message(NOTFOUND_TEXTURE_MSG);
	choosing_direction_utils(first_part, second_part, checkDuplicate, path);
}

void	ceiling_floor_color(char *first_part, char *second_part, \
t_directions *path, t_checkDuplicate *checkDuplicate)
{
	if (first_part[0] == FLOOR)
	{
		counter_checker(&checkDuplicate->fcounter, FLOOR_DUPLICATE);
		free (first_part);
		path->floor_color = checking_rgb_format(second_part);
		free (second_part);
		checkDuplicate->fcounter = 1;
	}
	else
	{
		counter_checker(&checkDuplicate->ccounter, CEILING_DUPLICATE);
		free (first_part);
		path->ceiling_color = checking_rgb_format(second_part);
		free (second_part);
		checkDuplicate->ccounter = 1;
	}
}

int	get_first_index(char *entereddata)
{
	int	i;

	i = 0;
	while (entereddata[i] && entereddata[i] != SPACE)
		i++;
	while (entereddata[i] && entereddata[i] == SPACE)
		i++;
	return (i);
}

void	map_first_part(char *entereddata, t_directions *path, \
t_checkDuplicate *checkDuplicate)
{
	int		first_index;
	char	*first_part;
	char	*second_part;

	first_part = ft_substr(entereddata, 0, 2);
	first_index = get_first_index(entereddata);
	second_part = ft_substr(entereddata, first_index, \
	ft_strlen(entereddata) - ft_strlen(first_part));
	free (entereddata);
	if (!ft_strncmp(first_part, "WE") || !ft_strncmp(first_part, "EA") \
	||!ft_strncmp(first_part, "NO") || !ft_strncmp(first_part, "SO"))
		choosing_directions(first_part, second_part, path, checkDuplicate);
	else if (!ft_strncmp(first_part, "C ") || !ft_strncmp(first_part, "F "))
		ceiling_floor_color(first_part, second_part, path, checkDuplicate);
	else
		error_message(INVALID_DIRECTION_MSG);
}
