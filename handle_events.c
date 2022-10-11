/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:28:53 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	next_frame(int key, t_player *player)
{
	if (key == 48 && player->mouse == 0)
		player->mouse = 1;
	else if (key == 48 && player->mouse == 1)
		player->mouse = 0;
	if (key == 53)
	{
		free(player->ray);
		free(player->width_for_each);
		exit(1);
	}
	init_keys(player, key);
	mlx_clear_window(player->image.mlx, player->image.win);
	change_player_status(player, key);
	get_rays(player);
	render_3d(player);
	mlx_put_image_to_window(player->image.mlx, \
		player->image.win, player->img.img, 0, 0);
	get_value_back(player);
	return (0);
}

int	stop(void)
{
	return (0);
}

int	close_win(t_player *player)
{
	free(player->width_for_each);
	free(player->ray);
	exit(1);
}
