/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:26:09 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/10 14:42:14 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	choosing_direction_utils(char *firstPart, char *secondPart, t_checkDuplicate *vars, t_directions *path)
{
	if (!ft_strncmp(firstPart, _north))
	{
		texturescounterChecker(&vars->nocounter, north_TEXTURE_DUPLICATE);
		path->north = secondPart;
	}
	else if (!ft_strncmp(firstPart, _west))
	{
		texturescounterChecker(&vars->wecounter, west_TEXTURE_DUPLICATE);
		path->west = secondPart;
	}
	else if (!ft_strncmp(firstPart, _east))
	{
		texturescounterChecker(&vars->eacounter, east_TEXTURE_DUPLICATE);
		path->east = secondPart;
	}
	else if(!ft_strncmp(firstPart, _south))
	{
		texturescounterChecker(&vars->socounter, south_TEXTURE_DUPLICATE);
		path->south = secondPart;
	}
	else
		errorMessage(INVALID_DIRECTION_MSG);
}

void	choosing_directions(char *firstPart,char *secondPart, t_directions *path, t_checkDuplicate *checkDuplicate)
{
	
	if (open(secondPart, R_OK) == -1)
	{
		free(firstPart);
		free(secondPart);
		errorMessage(NOTFOUND_TEXTURE_MSG);
	}
	choosing_direction_utils(firstPart, secondPart, checkDuplicate, path);
}

void	ceilingFloorColor(char *firstPart, char *secondPart,t_directions *path, t_checkDuplicate *checkDuplicate)
{
	(void)checkDuplicate;
	if (firstPart[0] == FLOOR)
	{
		path->floor_color = checkingRGBFormat(secondPart);
		checkDuplicate->fcounter = 1;
	}
	else
	{
        path->ceiling_color = checkingRGBFormat(secondPart);
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

void    map_first_part(char *entereddata, t_directions *path, t_checkDuplicate *checkDuplicate)
{
	int firstIndex;
    char *firstPart;
	char *secondPart;
	
    firstPart = ft_substr(entereddata, 0, 2);
	firstIndex = get_first_index(entereddata);
	secondPart = ft_substr(entereddata, firstIndex, ft_strlen(entereddata) - ft_strlen(firstPart));
    if (!ft_strncmp(firstPart, "WE") || !ft_strncmp(firstPart, "EA") ||\
	 !ft_strncmp(firstPart, "NO") || !ft_strncmp(firstPart, "SO"))
    	choosing_directions(firstPart, secondPart, path, checkDuplicate);
    else if (!ft_strncmp(firstPart, "C ") || !ft_strncmp(firstPart, "F "))
		ceilingFloorColor(firstPart, secondPart, path, checkDuplicate);
    else
		errorMessage(INVALID_DIRECTION_MSG);
}

void	get_player_position(t_directions *path)
{
	int	i;
	int	j;

	i = 0;
	while (path->map[i])
	{
		j = 0;
		while (path->map[i][j])
		{
			if (path->map[i][j] == 'N' || path->map[i][j] == 'S' || path->map[i][j] == 'W' || path->map[i][j] == 'E')
			{
				path->startposition = path->map[i][j];
				path->player_x = j;
				path->player_y = i;
				return ;
			}
			j++;
		}
		i++;
	}
}

void	map_second_part(int fd, t_directions *path)
{	
	t_secondPartVars vars;

	second_part_init(&vars);
	while (TRUE)
	{
		vars.entereddata = get_next_line(fd);
		if (!vars.entereddata)
			break ;
		if (ft_strncmp(vars.entereddata, "\n"))
			vars.counter++;
		unwanted_characters(&vars);
		vars.map = ft_strjoin(vars.map, vars.entereddata);
		free (vars.entereddata);
	}
	if (vars.counter < 3)
		errorMessage(INVALID_MAP);
	if (vars.isplayerexist == 0)
		errorMessage(MISSING_PLAYER_MSG);
	while (vars.map[vars.i] && vars.map[vars.i] == NEW_LINE)
		vars.i++;
	path->map = ft_split(vars.map + vars.i, NEW_LINE);
	get_player_position(path);
	check_for_double_newlines(vars.map + vars.i);
	mapValidation(path);
}
void	space_skipping(char *map, int *index)
{
	while (map[*index] && map[*index] == SPACE)
		(*index)++;
}

void	gettingMapContent(int fd, t_directions *path)
{
	t_mapContentVars vars;
	t_checkDuplicate checkDuplicate;

	checking_duplicate_init(&checkDuplicate);
	map_content_init(&vars);
	while (vars.linescounter < 6)
	{
		vars.entereddata = get_next_line(fd);
		if (!vars.entereddata)
			break ;
		if (ft_strncmp(vars.entereddata, "\n"))
		{
			if (vars.entereddata[ft_strlen(vars.entereddata) - 1] == NEW_LINE)
				vars.entereddata[ft_strlen(vars.entereddata) - 1] = '\0';
			vars.trimmedData = ft_strtrim(vars.entereddata, " ");
			vars.linescounter++;
        	map_first_part(vars.trimmedData, path, &checkDuplicate);
		}
		free (vars.entereddata);
		vars.i++;
	}
	if (vars.linescounter < 6)
		errorMessage(INVALID_MAP);
	missingTexture(&checkDuplicate);
	map_second_part(fd, path);
}


int main(int ac, char **av)
{
	int fd;
	char	*cub_extension;
	t_directions	path;
	
	path_init(&path);
	cub_extension = search(av[1], '.');
	if (cub_extension && ft_strncmp(cub_extension, ".cub") == 0 && ac == 2)
	{
		fd = open(av[1], 2);
		gettingMapContent(fd, &path);
		start_game(&path);
	}
	else
		errorMessage(ERROR_OCCURRED);
}
