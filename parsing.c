/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:26:09 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
			if (path->map[i][j] == 'N' || path->map[i][j] == 'S' \
			|| path->map[i][j] == 'W' || path->map[i][j] == 'E')
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
	t_secondPartVars	vars;

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
		error_message(INVALID_MAP);
	if (vars.isplayerexist == 0)
		error_message(MISSING_PLAYER_MSG);
	while (vars.map[vars.i] && vars.map[vars.i] == NEW_LINE)
		vars.i++;
	path->map = ft_split(vars.map + vars.i, NEW_LINE);
	get_player_position(path);
	check_for_double_newlines(vars.map + vars.i);
	free (vars.map);
	map_validation(path);
}

void	space_skipping(char *map, int *index)
{
	while (map[*index] && map[*index] == SPACE)
		(*index)++;
}

void	getting_map_content(int fd, t_directions *path)
{
	t_mapContentVars	vars;
	t_checkDuplicate	check_duplicate;

	checking_duplicate_init(&check_duplicate);
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
			vars.trimmeddata = ft_strtrim(vars.entereddata, " ");
			vars.linescounter++;
			map_first_part(vars.trimmeddata, path, &check_duplicate);
		}
		free (vars.entereddata);
		vars.i++;
	}
	if (vars.linescounter < 6)
		error_message(INVALID_MAP);
	map_second_part(fd, path);
}

int	main(int ac, char **av)
{
	int				fd;
	char			*cub_extension;
	t_directions	path;

	path_init(&path);
	cub_extension = search(av[1], '.');
	if (cub_extension && ft_strncmp(cub_extension, ".cub") == 0 && ac == 2)
	{
		fd = open(av[1], 2);
		if (fd == -1)
			error_message(PATH_ERROR);
		getting_map_content(fd, &path);
		start_game(&path);
	}
	else
		error_message(ERROR_OCCURRED);
}
