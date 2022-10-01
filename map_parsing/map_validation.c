/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/06 11:52:30 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/09/10 15:24:39 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void    unwanted_characters(char **map, t_directions *path)
{
    int i;
    int j;
    int is_exist;

    i = 0;
    is_exist = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (map[i][j] == 'N' || map[i][j] == 'S' || map[i][j] == 'W' || map[i][j] == 'E')
            {
                if (is_exist == 0)
				{
					path->PLAYER_Y = i;
					path->PLAYER_X = j;
					path->START_POSITION = map[i][j];
                    is_exist = 1;
				}
                else
                    error_message("\033[0;31mMap Parsing Error: Map Mustn't Contain More Than 1 Player");
            }
            else if (map[i][j] != ZERO && map[i][j] != ONE && map[i][j] != SPACE)
                error_message("\033[0;31mMap Parsing Error: Unwanted Character Exist");
            j++;
        }
        i++;
    }
}

void	checking_rgb_format_init(t_checking_rgb *var)
{
	var->i = 0;
	var->base = 16;
	var->commas_counter = 0;
	var->start = 0;
	var->end = 0;
	var->value = 0;
	var->number = NULL;
}

int	checking_rgb_format(char *directions)
{
	t_checking_rgb var;

	checking_rgb_format_init(&var);
	if ((directions[var.i] != FLOOR && directions[var.i] != CEILLING) || directions[var.i + 1] != SPACE)
		error_message("\033[0;31mMap Parsing Error: Floor Or Ceilling Color Missing");
	var.i+=2;
	checking_rgb_format_utils(directions, &var);
	var.number = ft_substr(directions, var.start, var.end);
	var.color_number = ft_atoi(var.number);
	var.value += var.color_number;
	if (!(var.end >= 1 && var.end <= 3) || var.commas_counter != 2 || !(var.color_number >= 0 && var.color_number <= 255))
	{
		free (var.number);
		error_message("\033[0;31mMap Parsing Error: Wrong RGB Format");
	}
	if (var.number)
		free (var.number);
	return (var.value);
}

void	is_surrounded_by_walls(char **map, int map_height)
{
	int	string_length;
	int	i;
	int	j;
	char	*trimmed;

	i = 0;
	string_length = 0;
	while (map[i])
	{
		trimmed = ft_strtrim(map[i], " ");
		string_length = ft_strlen(trimmed) - 1;
		if (trimmed[0] != '1' || trimmed[string_length] != '1')
			error_message("\033[0;31mMap Parsing Error: Map Should Be Surrounded By Walls");
		if (i == 0 || i == map_height - 1)
		{
			j = 0;
			while (trimmed[j])
			{
				if (trimmed[j] != '1' && trimmed[j] != SPACE)
					error_message("\033[0;31mMap Parsing Error: Map Should Be Surrounded By Walls");
				j++;
			}
		}
		free (trimmed);
		i++;
	}
}

void	correct_position_for_space(char **map, size_t map_height)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (map[i])
	{
        j = 0;
        while (map[i][j])
		{
			if (map[i][j] == ZERO)
			{
                if ((i < map_height - 1 && j > ft_strlen(map[i + 1]) - 1) || (i < map_height - 1 && j > ft_strlen(map[i - 1]) - 1))
				{
					// printf("i is : %d and j is : %d, map_height %d, width : %lu\n", i, j, map_height, ft_strlen(map[i - 1]) - 1);
                    error_message("\033[0;31mMap Parsing Error: Space Should Be Surrounded By Walls");
				}
				if ((i > 0 && map[i - 1][j] == SPACE) || (i < map_height - 1 && map[i + 1][j] == SPACE))
					error_message("\033[0;31mMap Parsing Error: Space Should Be Surrounded By Walls");
				if ((map[i][j + 1] == SPACE) || (j > 0 && map[i][j - 1] == SPACE))
					error_message("\033[0;31mMap Parsing Error: Space Should Be Surrounded By Walls");
			}
			j++;
		}
		i++;
	}
}

void	map_validation(t_directions *path)
{
	size_t	map_height;

	map_height = get_size(path->map);
	is_surrounded_by_walls(path->map, map_height);
	correct_position_for_space(path->map, map_height);
    unwanted_characters(path->map, path);
}