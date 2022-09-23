/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:26:09 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/09/13 10:46:59 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	checking_rgb_format_utils(char *directions, t_checking_rgb *var)
{
	while (directions[var->i])
	{
		var->color_number = 0;
		if (ft_isdigit(directions[var->i]))
			{
				var->start = var->i++;
				while (ft_isdigit(directions[var->i]))
					var->i++;
				var->end = var->i - var->start;
				continue;
			}
			else if (directions[var->i] == COMMA)
			{
				var->commas_counter++;
				var->number = ft_substr(directions, var->start, var->end);
				var->color_number = ft_atoi(var->number);
				if (!(var->end >= 1 && var->end <= 3) || var->commas_counter > 2 || !(var->color_number >= 0 && var->color_number <= 255))
					error_message("\033[0;31mMap Parsing Error: Wrong RGB Format");
				var->value += var->color_number << var->base;
				var->base -= 8;
				free (var->number);
			}
			else
				error_message("\033[0;31mMap Parsing Error: Wrong RGB Format");
		var->i++;
	}
}
	
void	ceiling_floor_color(char *directions, t_directions *path)
{
	if (directions[0] == FLOOR)
		path->FLOOR_COLOR = checking_rgb_format(directions);
	else
		path->CEILING_COLOR = checking_rgb_format(directions);
}

int	get_size(char **data)
{
	int	i;

	i = 0;
	while (data[i])
		i++;
	return (i);
}

void	from_tab_to_space(char *data)
{
	int	i;

	i = 0;
	while (data[i])
	{
		if (data[i] == TAB)
			data[i] = SPACE;
		i++;
	}
}

void	getting_textures_utils(t_directions *path, t_getting_textures *vars)
{
	while (vars->i < 6)
	{
		vars->s = ft_split(vars->data[vars->i], SPACE);
		if (ft_strlen(vars->s[0]) == 1)
			ceiling_floor_color(vars->data[vars->i], path);
		else
			choosing_directions(vars->data[vars->i], path);
		vars->i++;
		ft_free(vars->s);
	}
}

void	getting_textures(char *map, t_directions *path)
{
	
	t_getting_textures vars;

	vars.i = 0;
	vars.data = ft_split(map, NEW_LINE);
	vars.map_size = get_size(vars.data);
	path->map = malloc (sizeof(char *) * (vars.map_size - 6 + 1));
	init(path);
	getting_textures_utils(path, &vars);
	while (vars.data[vars.i])
	{
		from_tab_to_space(vars.data[vars.i]);
		path->map[vars.i - 6] = ft_strdup(vars.data[vars.i]);
		vars.i++;
	}
	path->map[vars.i - 6] = NULL;
	ft_free(vars.data);
	map_validation(path);
}

void	getting_map_content(int fd, t_directions *path)
{
	char	*map;
	char	*data;
	int i;

	i = 0;
	map = NULL;
	while (TRUE)
	{
		data = get_next_line(fd);
		if (!data)
			break ;
		map = ft_strjoin(map, data);
		free (data);
		i++;
	}
	getting_textures(map, path);
	free (map);
}


int main(int ac, char **av)
{
	int fd;
	char	*cub_extension;
	t_directions	path;

	cub_extension = search(av[1], '.');
	if (cub_extension && ac == 2)
	{
		if (ft_strncmp(cub_extension, ".cub") == 0)
		{
			fd = open(av[1], 2);
			getting_map_content(fd, &path);
			start_game(&path);
		}
	}
	else
		error_message("Error occured");
}