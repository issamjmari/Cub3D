/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_variables.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:06:03 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/10 14:41:52 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_keys(t_player *player, int key)
{
	if (key == 13 || key == 126)
		player->walkdirection = 1;
	if (key == 123)
		player->turndirection = -1;
	if (key == 0)
		player->movedirection = 1;
	if (key == 2)
		player->movedirection = -1;
	if (key == 1 || key == 125)
		player->walkdirection = -1;
	if (key == 124)
		player->turndirection = 1;
}

float	get_init_pos(t_directions *path)
{
	if (path->startposition == 'N')
		return ((3 * M_PI) / 2);
	if (path->startposition == 'W')
		return (M_PI);
	if (path->startposition == 'S')
		return ((M_PI / 2));
	if (path->startposition == 'E')
		return (0);
	return (0);
}

void	init_player(t_directions *path, t_player *player)
{
	player->x = (path->player_x + 0.5) * 64;
	player->y = (path->player_y + 0.5) * 64;
	player->mouse = 0;
	player->height = get_height(path);
	player->width = get_width(path);
	player->width_for_each = get_widths(path, player->height);
	player->turndirection = 0;
	player->walkdirection = 0;
	player->tab_press = 0;
	player->rotationangle = get_init_pos(path);
	player->walkspeed = 0.60 * 64;
	player->turnspeed = 10 * (M_PI / 180);
	player->image.mlx = mlx_init();
	player->image.win = mlx_new_window(player->image.mlx, 1500, 900, "Cub3d");
	player->data = path;
	player->ray = malloc(1500 * sizeof(t_ray));
}

void	get_value_back(t_player *player)
{
	player->walkdirection = 0;
	player->turndirection = 0;
	player->movedirection = 0;
}

void	set_player(float a, float b, t_player *player)
{
	player->x = a;
	player->y = b;
}
