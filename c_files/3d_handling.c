/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3d_handling.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:14:54 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/10 14:40:34 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	handle_multiple3d(t_threed_handle *data, t_player *player, \
int index, float project_height)
{
	int	yoffset;

	while (data->ceil_y < data->put_pos)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		player->data->ceiling_color);
	while (data->ceil_y < data->floor_y)
	{
		yoffset = ((data->loop++ + ((project_height / 2) - (900 / 2))) * \
	((float)(64 / project_height)));
		if (yoffset < 0)
			yoffset = 0;
		if (yoffset > 63 || data->xoffset > 63)
			break ;
		put_image_strip(player, index, data, yoffset);
	}
	while (data->floor_y < 900)
		my_mlx_pixel_put(&player->img, index, data->floor_y++, \
		player->data->floor_color);
}

void	handle_single3d(t_threed_handle *data, t_player *player, \
int index, float project_height)
{
	int	y;
	int	yoffset;

	y = 0;
	while (y < 900)
	{
		yoffset = (data->loop++ + ((project_height / 2) - (900 / 2))) \
	* ((float)(64 / project_height));
		if (yoffset < 0)
			yoffset = 0;
		if (yoffset > 63 || data->xoffset > 63)
			break ;
		if (player->ray[index].ray_content == 1)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->xoffset, &player->img1));
		else if (player->ray[index].ray_content == 2)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->xoffset, &player->img2));
		else if (player->ray[index].ray_content == 3)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->xoffset, &player->img3));
		else if (player->ray[index].ray_content == 4)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->xoffset, &player->img4));
	}
}

void	put_stripin3d(t_player *player, float project_height, int index)
{
	t_threed_handle	data;

	if (player->ray[index].was_hit_vertical)
		data.xoffset = (int)(player->ray[index].wallhity) % 64;
	else
		data.xoffset = (int)(player->ray[index].wallhitx) % 64;
	data.put_pos = (int)((900 / 2) - (project_height / 2));
	if (data.put_pos < 0)
		data.put_pos = 0;
	data.ceil_y = 0;
	data.floor_y = data.put_pos + project_height;
	data.loop = data.put_pos;
	if (data.put_pos > 0)
		handle_multiple3d(&data, player, index, project_height);
	else
		handle_single3d(&data, player, index, project_height);
}

void	render_3d(t_player *player)
{
	int		i;
	float	distance_toprojection;
	float	projection_wall_height;

	i = 0;
	distance_toprojection = 750 / tan(((60 * (M_PI / 180)) / 2));
	while (i < 1500)
	{
		projection_wall_height = (64 / (player->ray[i].distance * \
	cos(player->ray[i].ray_angle - player->rotationangle))) \
	* distance_toprojection;
		put_stripin3d(player, projection_wall_height, i);
		i++;
	}
}
