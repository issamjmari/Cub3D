/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:28:53 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/06 20:53:26 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	ft_draw_elem(int x, int y, t_player *player, int color)
{
	int end_y = ((y) + 10);
	int end_x = ((x) + 10);
	int start_x;
	int start_y = (y);
	while (start_y < end_y)
	{
		start_x = (x);
		while (start_x < end_x)
		{
			my_mlx_pixel_put(&player->img, start_x, start_y, color);
			start_x++;
		}
		start_y++;
	}
	draw_line2(82, 82, 82 + 30 * cos(player->rotationAngle), 82 + 30 * sin(player->rotationAngle), player);
}

void	render_minimap(t_player *player)
{
	int		i;
	float	angle;

	i = 0;
	angle = (M_PI / 2);
	while (i < 1000)
	{
		angle = remainder(angle, 2 * M_PI);
		draw_line1(player, player->x + 100 * cos(angle), player->y + 100 * sin(angle), angle);
		angle += ((360 * (M_PI / 180)) / 1000);
		i++;
	}
	ft_draw_elem(80, 80, player, 0x0000FF);
}

int	next_frame(int key, t_player *player)
{
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
	render_minimap(player);
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
