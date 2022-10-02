/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:03:46 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/02 14:24:10 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	iswall(float a, float b, t_player *player)
{
	int	wallcheckx;
	int	wallchecky;

	if (a < 0 || a > player->width * 64
		|| b < 0 || b > player->height * 64)
		return (1);
	wallcheckx = floor(a / 64);
	wallchecky = floor(b / 64);
	if (wallchecky >= player->height
		|| wallcheckx >= player->width_for_each[wallchecky]
		|| wallchecky < 0 || wallcheckx < 0)
		return (1);
	return (player->data->map[wallchecky][wallcheckx] == '1');
}

void	change_player_status(t_player *player)
{
	float	newrotation;
	float	movea;
	float	moveb;
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	if (player->rotationAngle != M_PI / 2)
		newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(player->rotationAngle) * \
	(player->walkDirection * player->walkSpeed);
	b = player->y + sin(player->rotationAngle) * \
	(player->walkDirection * player->walkSpeed);
	if (!iswall(a, b, player))
		set_player(a, b, player);
	if (player->moveDirection != 0)
	{
		movea = player->x + (15 * player->moveDirection * cos(newrotation));
		moveb = player->y + (15 * player->moveDirection * sin(newrotation));
		if (!iswall(movea, moveb, player))
			set_player(movea, moveb, player);
	}
}

void	start_game(t_directions *path)
{
	t_player	player;

	init_player(path, &player);
	create_images(&player);
	get_rays(&player);
	render_3d(&player);
	mlx_put_image_to_window(player.image.mlx, player.image.win, \
	player.img.img, 0, 0);
	mlx_hook(player.image.win, 2, 0, next_frame, &player);
	mlx_hook(player.image.win, 3, 0, stop, NULL);
	mlx_hook(player.image.win, 17, (1L << 15), close_win, &player);
	mlx_loop(player.image.mlx);
}
