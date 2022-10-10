/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapValidation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 11:52:30 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/03 14:48:38 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	checking_rgb_number(char *secondPart, t_checking_rgb *var)
{
	var->digitscounter++;
	var->start = var->i;
	while (ft_isdigit(secondPart[var->i]))
		var->i++;
	var->end = var->i - var->start;
	var->number = ft_substr(secondPart, var->start, var->end);
	var->colornumber = ft_atoi(var->number);
	if (var->end > 3 || var->colornumber < 0 || var->colornumber > 255)
		errorMessage(WRONG_RGB_MSG);
	var->value += var->colornumber << var->base;
	var->base -= 8;
}

int	checkingRGBFormat(char *secondPart)
{
	t_checking_rgb var;

	rgb_init(&var);
	while (secondPart[var.i])
	{
		if (ft_isdigit(secondPart[var.i]))
		{
			checking_rgb_number(secondPart, &var);
			continue;
			if (!secondPart[var.i])
				break ;
		}
		else if (secondPart[var.i] == COMMA)
		{
			if (var.commascounter == 2)
				errorMessage(WRONG_RGB_MSG);
			var.commascounter++;
		}
		else if (secondPart[var.i] != SPACE)
			errorMessage(WRONG_RGB_MSG);
		var.i++;
	}
	if (var.digitscounter != 3 || var.commascounter != 2)
		errorMessage(WRONG_RGB_MSG);
	return (var.value);
}

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

void    correctPositionForSpaceUtils(int i, int map_height, char **map)
{
    size_t j;

    j = 0;
    while (map[i][j])
    {
        if (map[i][j] == ZERO)
        {
            if ((i < map_height - 1 && j > ft_strlen(map[i + 1]) - 1) \
            || (i < map_height - 1 && j > ft_strlen(map[i - 1]) - 1))
                errorMessage(SURROUNDED_SPACE_MSG);
            if ((i > 0 && map[i - 1][j] == SPACE) \
            || (i < map_height - 1 && map[i + 1][j] == SPACE))
                errorMessage(SURROUNDED_SPACE_MSG);
            if ((map[i][j + 1] == SPACE) \
            || (j > 0 && map[i][j - 1] == SPACE))
                errorMessage(SURROUNDED_SPACE_MSG);
        }
        j++;
    }
}

void	correctPositionForSpace(char **map, size_t map_height)
{
	size_t	i;

	i = 0;
	while (map[i])
	{
        correctPositionForSpaceUtils(i, map_height, map);
		i++;
	}
}

void	mapValidation(t_directions *path)
{
	size_t	map_height;

	map_height = getSize(path->map);
	isSurroundedByWalls(path->map, map_height);
	correctPositionForSpace(path->map, map_height);
}
