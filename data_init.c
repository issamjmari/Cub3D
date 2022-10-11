/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 10:47:40 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	checking_duplicate_init(t_checkDuplicate *checkDuplicate)
{
	checkDuplicate->nocounter = 0;
	checkDuplicate->ccounter = 0;
	checkDuplicate->wecounter = 0;
	checkDuplicate->socounter = 0;
	checkDuplicate->eacounter = 0;
	checkDuplicate->fcounter = 0;
}

void	map_content_init(t_mapContentVars *vars)
{
	vars->i = 0;
	vars->linescounter = 0;
	vars->trimmeddata = NULL;
}

void	second_part_init(t_secondPartVars *vars)
{
	vars->i = 0;
	vars->entereddata = NULL;
	vars->map = NULL;
	vars->isplayerexist = 0;
	vars->counter = 0;
}

void	path_init(t_directions *path)
{
	path->north = NULL;
	path->east = NULL;
	path->west = NULL;
	path->south = NULL;
	path->ceiling_color = 0;
	path->floor_color = 0;
}

void	rgb_init(t_checking_rgb *var)
{
	var->digitscounter = 0;
	var->base = 16;
	var->colornumber = 0;
	var->commascounter = 0;
	var->end = 0;
	var->i = 0;
	var->number = NULL;
	var->start = 0;
	var->value = 0;
}
