/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 14:10:59 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	check_left_angle(t_player *player)
{
	float	a;
	float	b;

	player->rotationangle += player->turndirection * player->turnspeed;
	a = player->x + cos(player->rotationangle) * -1;
	b = player->y + sin(player->rotationangle) * -1;
	if (!iswall(a, b, player) && !check_line(player, player->x - 30 * \
		cos(player->rotationangle), player->y - 30 * \
		sin(player->rotationangle)))
		set_player(a, b, player);
}

void	check_right_angle(t_player *player)
{
	float	a;
	float	b;

	player->rotationangle += player->turndirection * player->turnspeed;
	a = player->x + cos(player->rotationangle);
	b = player->y + sin(player->rotationangle);
	if (!iswall(a, b, player) && !check_line(player, player->x + 30 * \
	cos(player->rotationangle), player->y + 30 * sin(player->rotationangle)))
		set_player(a, b, player);
}

void	check_right_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;

	player->rotationangle += player->turndirection * player->turnspeed;
	newrotation = player->rotationangle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10;
	b = player->y + sin(newrotation) * 10;
	if (!iswall(a, b, player) && !check_line(player, \
	player->x + 30 * cos(newrotation) \
	, player->y + 30 * sin(newrotation)))
		set_player(a, b, player);
}

void	check_left_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;

	player->rotationangle += player->turndirection * player->turnspeed;
	newrotation = player->rotationangle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10 * -1;
	b = player->y + sin(newrotation) * 10 * -1;
	if (!iswall(a, b, player) && \
	!check_line(player, player->x - 30 * cos(newrotation) \
	, player->y - 30 * sin(newrotation)))
		set_player(a, b, player);
}

void	change_player_status(t_player *player, int key)
{
	if (key == 13 || key == 126)
		check_up(player);
	if (key == 123)
		check_left_angle(player);
	if (key == 0)
		check_right_move(player);
	if (key == 2)
		check_left_move(player);
	if (key == 1 || key == 125)
		check_down(player);
	if (key == 124)
		check_right_angle(player);
}
