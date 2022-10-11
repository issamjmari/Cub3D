/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:32:16 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_full_spaces(char *entereddata)
{
	int	i;

	i = 0;
	while (entereddata[i])
	{
		if (entereddata[i] != SPACE)
			return (0);
		i++;
	}
	return (1);
}

void	is_surrounded_by_walls_utils(char *trimmed)
{
	int	j;

	j = 0;
	while (trimmed[j])
	{
		if (trimmed[j] != '1' && trimmed[j] != SPACE)
			error_message(SURROUNDED_MSG);
	j++;
	}
}

void	is_surrounded_by_walls(char **map, int map_height)
{
	int		string_length;
	int		i;
	char	*trimmed;

	i = 0;
	string_length = 0;
	while (map[i])
	{
		trimmed = ft_strtrim(map[i], " ");
		string_length = ft_strlen(trimmed) - 1;
		if (is_full_spaces(trimmed))
		{
			i++;
			free (trimmed);
			continue ;
		}
		else if (trimmed[0] != '1' || trimmed[string_length] != '1')
			error_message(SURROUNDED_MSG);
		if (i == 0 || i == map_height - 1)
			is_surrounded_by_walls_utils(trimmed);
		free (trimmed);
		i++;
	}
}

int	check_for_double_newlines(char *map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (map[i] == NEW_LINE && map[i + 1] == NEW_LINE)
			error_message(DOUBLE_NEW_LINE);
		i++;
	}
	return (0);
}

void	unwanted_characters(t_secondPartVars *vars)
{
	int	j;

	j = 0;
	while (vars->entereddata[j])
	{
		if (vars->entereddata[j] == 'N' || vars->entereddata[j] == 'S' || \
		vars->entereddata[j] == 'W' || vars->entereddata[j] == 'E')
		{
			if (vars->isplayerexist == 0)
				vars->isplayerexist = 1;
			else
				error_message(CONTAIN_MORE_MSG);
		}
		else if (vars->entereddata[j] != ZERO && \
		vars->entereddata[j] != ONE && vars->entereddata[j] != \
		SPACE && vars->entereddata[j] != NEW_LINE)
			error_message(UNWANTED_CHARACTER_MSG);
		j++;
	}
}
