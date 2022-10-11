/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:10:06 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/11 22:01:25 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}

unsigned int	get_color(int y, int x, t_data *img)
{
	char	*dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

void	put_image_strip(t_player *player, int index, \
t_threed_handle *data, int yoffset)
{
	if (player->ray[index].ray_content == 1)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
	get_color(yoffset, data->xoffset, &player->img1));
	else if (player->ray[index].ray_content == 2)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->xoffset, &player->img2));
	else if (player->ray[index].ray_content == 3)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->xoffset, &player->img3));
	else if (player->ray[index].ray_content == 4)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->xoffset, &player->img4));
}
