/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:10:06 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/07 15:58:40 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_line2(float begx, float begy, float endx, float endy, t_player *player)
{
	double pixelX;
	double pixelY;
	double deltaX;
	double deltaY;
	int pixels;

	deltaX = (endx - begx);
	deltaY = (endy - begx);
	pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;
	pixelX = begx;
	pixelY = begy;
	while (pixels)
	{
		my_mlx_pixel_put(&player->img, pixelX, pixelY, 0x66FF0000);
		pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}

void	draw_line1(t_player *player, float endX, float endY, float angle)
{
	double pixelX;
	double pixelY;
	double deltaX;
	double deltaY;
	double start_circlex = 140;
	double start_circley = 140;
	int pixels;

	deltaX = (endX - player->x);
	deltaY = (endY - player->y);
	pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;
	pixelX = player->x;
	pixelY = player->y;
	while (pixels)
	{
	    if(iswall(pixelX, pixelY, player))
			my_mlx_pixel_put(&player->img, start_circlex + cos(angle), start_circley + sin(angle), 0x00FF0000);
		else
			my_mlx_pixel_put(&player->img, start_circlex + cos(angle), start_circley + sin(angle), 0xE9967A);
		start_circlex += cos(angle);
		start_circley += sin(angle);
		pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}

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
	get_color(yoffset, data->Xoffset, &player->img1));
	else if (player->ray[index].ray_content == 2)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->Xoffset, &player->img2));
	else if (player->ray[index].ray_content == 3)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->Xoffset, &player->img3));
	else if (player->ray[index].ray_content == 4)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		get_color(yoffset, data->Xoffset, &player->img4));
}
