/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:03:46 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int	check_line(t_player *player, float endX, float endY)
{
	double	pixelx;
	double	pixely;
	double	deltax;
	double	deltay;
	int		pixels;

	deltax = (endX - player->x);
	deltay = (endY - player->y);
	pixels = sqrt((deltax * deltax) + (deltay * deltay));
	deltax /= pixels;
	deltay /= pixels;
	pixelx = player->x;
	pixely = player->y;
	while (pixels)
	{
		if (iswall(pixelx, pixely, player))
			return (1);
		pixelx += deltax;
		pixely += deltay;
		--pixels;
	}
	return (0);
}

void	check_up(t_player *player)
{
	float	a;
	float	b;

	player->rotationangle += player->turndirection * player->turnspeed;
	a = player->x + cos(player->rotationangle) * \
	player->walkspeed;
	b = player->y + sin(player->rotationangle) * \
	player->walkspeed;
	if (!iswall(a, b, player) && !check_line(player, player->x + 50 * \
		cos(player->rotationangle), player->y + 50 * sin(player->rotationangle))
		&& !check_line(player, player->x + 50 * \
		cos(player->rotationangle - (10 * M_PI / 180)), player->y + 50 * \
		sin(player->rotationangle - (10 * M_PI / 180)))
		&& !check_line(player, player->x + 50 * \
		cos(player->rotationangle + (10 * M_PI / 180)), player->y + 50 * \
		sin(player->rotationangle + (10 * M_PI / 180))))
		set_player(a, b, player);
}

void	check_down(t_player *player)
{
	float	a;
	float	b;

	player->rotationangle += player->turndirection * player->turnspeed;
	a = player->x + cos(player->rotationangle) * \
	player->walkspeed * -1;
	b = player->y + sin(player->rotationangle) * \
	player->walkspeed * -1;
	if (!iswall(a, b, player) && !check_line(player, player->x - 50 * \
		cos(player->rotationangle), player->y - 50 * sin(player->rotationangle))
		&& !check_line(player, player->x - 50 * \
		cos(player->rotationangle - (10 * M_PI / 180)), player->y - 50 * \
		sin(player->rotationangle - (10 * M_PI / 180)))
		&& !check_line(player, player->x - 50 * \
		cos(player->rotationangle + (10 * M_PI / 180)), player->y - 50 * \
		sin(player->rotationangle + (10 * M_PI / 180))))
		set_player(a, b, player);
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
