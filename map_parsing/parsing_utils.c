/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/09 11:15:12 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/09/09 13:51:14 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init(t_directions *path)
{
	path->NORTH = NULL;
	path->EAST = NULL;
	path->WEST = NULL;
	path->SOUTH = NULL;
	path->CEILING_COLOR = 0;
	path->FLOOR_COLOR = 0;		
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

void	choosing_directions(char *direction, t_directions *path)
{
	char	**data;
	int		i;
	char	*real_direction_path;

    data = ft_split(direction, SPACE);
    real_direction_path = NULL;
    i = 1;
    while (data[i])
        real_direction_path = ft_strjoin(real_direction_path, data[i++]);
    if (!ft_strncmp(data[0], "NO")) {
        path->NORTH = real_direction_path;
    }
	else if (!ft_strncmp(data[0], "WE"))
		path->WEST = real_direction_path;
	else if (!ft_strncmp(data[0], "EA"))
		path->EAST = real_direction_path;
	else if(!ft_strncmp(data[0], "SO"))
		path->SOUTH = real_direction_path;
	else
		error_message("\033[0;31mMap Parsing Error: Write A Correct Direction");
	ft_free (data);
}

int	get_allocation_size(char *data)
{
	int	i;
	int	counter;

	counter = 0;
	i = 0;
	if (data[i] != FLOOR && data[i] != CEILLING)
		error_message("\033[0;31mMap Parsing Error: Floor Or Ceilling Color Missing");
	i++;
	while (data[i])
	{
		if (!ft_isdigit(data[i]) && data[i] != COMMA && data[i] != SPACE)
			error_message("\033[0;31mMap Parsing Error: Wrong RGB Format");
		if (data[i] != SPACE)
			counter++;
		i++;
	}
	return (counter + 1);
}