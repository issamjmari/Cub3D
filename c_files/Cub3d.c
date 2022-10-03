/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:03:46 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/03 16:15:27 by ijmari           ###   ########.fr       */
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

int	check_line(t_player *player, float endX, float endY)
{
	double deltaX = (endX - player->x);
	double deltaY = (endY - player->y);
	int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;
	double pixelX = player->x;
	double pixelY = player->y;
	while (pixels)
	{
	   if(iswall(pixelX, pixelY, player))
			return (1);
	    pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
	return (0);
}

void	check_up(t_player *player)
{
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * \
	player->walkSpeed;
	b = player->y + sin(player->rotationAngle) * \
	player->walkSpeed;
	if (!iswall(a, b, player) && !check_line(player, player->x + 30 * \
		cos(player->rotationAngle), player->y + 20 * sin(player->rotationAngle))
		&& !check_line(player, player->x + 20 * \
		cos(player->rotationAngle - (15 * M_PI / 180)), player->y + 20 * sin(player->rotationAngle - (15 * M_PI / 180)))
		&& !check_line(player, player->x + 20 * \
		cos(player->rotationAngle + (15 * M_PI / 180)), player->y + 20 * sin(player->rotationAngle + (15 * M_PI / 180))))
			set_player(a, b, player);
}

void	check_down(t_player *player)
{
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * \
	player->walkSpeed * -1;
	b = player->y + sin(player->rotationAngle) * \
	player->walkSpeed * -1;
	if (!iswall(a, b, player) && !check_line(player, player->x - 20 * \
		cos(player->rotationAngle), player->y - 20 * sin(player->rotationAngle))
		&& !check_line(player, player->x - 20 * \
		cos(player->rotationAngle - (5 * M_PI / 180)), player->y - 20 * sin(player->rotationAngle - (5 * M_PI / 180)))
		&& !check_line(player, player->x - 20 * \
		cos(player->rotationAngle + (5 * M_PI / 180)), player->y - 20 * sin(player->rotationAngle + (5 * M_PI / 180))))
			set_player(a, b, player);
}

void	check_left_angle(t_player *player)
{
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * -1;
	b = player->y + sin(player->rotationAngle) * -1;
	if (!iswall(a, b, player) && !check_line(player, player->x - 30 * \
		cos(player->rotationAngle), player->y - 30 * sin(player->rotationAngle)))
		set_player(a, b, player);
}

void	check_right_angle(t_player *player)
{
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle);
	b = player->y + sin(player->rotationAngle);
	if (!iswall(a, b, player) && !check_line(player, player->x + 30 * \
		cos(player->rotationAngle), player->y + 30 * sin(player->rotationAngle)))
		set_player(a, b, player);
}

void	check_right_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10;
	b = player->y + sin(newrotation) * 10;
	if (!iswall(a, b, player))
		set_player(a, b, player);
}

void	check_left_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10 * -1;
	b = player->y + sin(newrotation) * 10 * -1;
	if (!iswall(a, b, player))
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
