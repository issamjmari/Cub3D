/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:03:46 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/08 16:15:14 by ijmari           ###   ########.fr       */
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
	float	temp_a;
	float	temp_b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * \
	player->walkSpeed;
	b = player->y + sin(player->rotationAngle) * \
	player->walkSpeed;
	temp_a = player->x + cos(player->rotationAngle) * \
	0.5f;
	temp_b = player->y + sin(player->rotationAngle) * \
	0.5f;
	if(!iswall(temp_a, temp_b, player))
	{
		player->temp_x = temp_a;
		player->temp_y = temp_b;
	}
	if (!iswall(a, b, player) && !check_line(player, player->x + 50 * \
		cos(player->rotationAngle), player->y + 50 * sin(player->rotationAngle))
		&& !check_line(player, player->x + 50 * \
		cos(player->rotationAngle - (10 * M_PI / 180)), player->y + 50 * \
		sin(player->rotationAngle - (10 * M_PI / 180)))
		&& !check_line(player, player->x + 50 * \
		cos(player->rotationAngle + (10 * M_PI / 180)), player->y + 50 * \
		sin(player->rotationAngle + (10 * M_PI / 180))))
			set_player(a, b, player);
}

void	check_down(t_player *player)
{
	float	a;
	float	b;
	float	temp_a;
	float	temp_b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * \
	player->walkSpeed * -1;
	b = player->y + sin(player->rotationAngle) * \
	player->walkSpeed * -1;
	temp_a = player->x + cos(player->rotationAngle) * \
	0.5f * -1;
	temp_b = player->y + sin(player->rotationAngle) * \
	0.5f * -1;
	if(!iswall(temp_a, temp_b, player))
	{
		player->temp_x = temp_a;
		player->temp_y = temp_b;
	}
	if (!iswall(a, b, player) && !check_line(player, player->x - 50 * \
		cos(player->rotationAngle), player->y - 50 * sin(player->rotationAngle))
		&& !check_line(player, player->x - 50 * \
		cos(player->rotationAngle - (10 * M_PI / 180)), player->y - 50 * \
		sin(player->rotationAngle - (10 * M_PI / 180)))
		&& !check_line(player, player->x - 50 * \
		cos(player->rotationAngle + (10 * M_PI / 180)), player->y - 50 * \
		sin(player->rotationAngle + (10 * M_PI / 180))))
			set_player(a, b, player);
}

void	check_left_angle(t_player *player)
{
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	a = player->x + cos(player->rotationAngle) * -1;
	b = player->y + sin(player->rotationAngle) * -1;
	if(!iswall(a, b, player))
	{
		player->temp_x = a;
		player->temp_y = b;
	}
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
	if(!iswall(a, b, player))
	{
		player->temp_x = a;
		player->temp_y = b;
	}
	if (!iswall(a, b, player) && !check_line(player, player->x + 30 * \
		cos(player->rotationAngle), player->y + 30 * sin(player->rotationAngle)))
		set_player(a, b, player);
}

void	check_right_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;
	float	temp_a;
	float	temp_b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10;
	b = player->y + sin(newrotation) * 10;
	temp_a = player->x + cos(newrotation);
	temp_b = player->y + sin(newrotation);
	if(!iswall(temp_a, temp_b, player))
	{
		player->temp_x = temp_a;
		player->temp_y = temp_b;
	}
	if (!iswall(a, b, player) && !check_line(player, player->x + 30 * cos(newrotation) \
	, player->y + 30 * sin(newrotation)))
		set_player(a, b, player);
}

void	check_left_move(t_player *player)
{
	float	a;
	float	b;
	float	newrotation;
	float	temp_a;
	float	temp_b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(newrotation) * 10 * -1;
	b = player->y + sin(newrotation) * 10 * -1;
	temp_a = player->x + cos(newrotation) * -1;
	temp_b = player->y + sin(newrotation) * -1;
	if (!iswall(temp_a, temp_b, player))
	{
		player->temp_x = temp_a;
		player->temp_y = temp_b;
	}
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

void	add_angle(t_player *player, int x, int y)
{
	if (x >= 0 && y >= 0 && x > 750)
	{
		player->rotationAngle += player->turnSpeed * 0.2;
		player->x += cos(player->rotationAngle);
		player->y += sin(player->rotationAngle);
	}
	else if (x >= 0 && y >= 0 && x < 750)
	{
		player->rotationAngle += player->turnSpeed * 0.2 * -1;
		player->x += cos(player->rotationAngle) * -1;
		player->y += sin(player->rotationAngle) * -1;
	}
}

int	mouse_handle(int x, int y, t_player *player)
{
	float	a;
	float	b;

	if (!player->mouse)
		return (1);
	if (x < 0 || x > 1500 || y < 0 || y > 900)
		return (1);
	if (x == 750)
		return (1);
	add_angle(player, x, y);
	mlx_clear_window(player->image.mlx, player->image.win);
	get_rays(player);
	render_3d(player);
	mlx_put_image_to_window(player->image.mlx, \
		player->image.win, player->img.img, 0, 0);
	return (0);
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
	mlx_hook(player.image.win, 6, 0, mouse_handle, &player);
	mlx_loop(player.image.mlx);
}
