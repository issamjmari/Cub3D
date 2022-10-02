/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:10:06 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/02 14:25:04 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *) dst = color;
}

void	ft_draw_elem(int x, int y, t_player *player, int color)
{
	int	end_y;
	int	end_x;
	int	start_x;
	int	start_y;

	start_y = (y * 64) * 0.3;
	end_y = ((y * 64) + 64) * 0.3;
	end_x = ((x * 64) + 64) * 0.3;
	while (start_y < end_y)
	{
		start_x = (x * 64) * 0.3 ;
		while (start_x < end_x)
		{
			my_mlx_pixel_put(&player->img, (start_x), (start_y), color);
			start_x++;
		}
		start_y++;
	}
}

void	draw_line(t_player *player, float endX, float endY, int color)
{
	float	deltax;
	float	deltay;
	int		pixels;
	float	pixelx;
	float	pixely;

	deltax = (endX - player->x);
	deltay = (endY - player->y);
	pixels = sqrt((deltax * deltax) + (deltay * deltay));
	deltax /= pixels;
	deltay /= pixels;
	pixelx = player->x;
	pixely = player->y;
	while (pixels)
	{
		my_mlx_pixel_put(&player->img, (pixelx * 0.3), \
	(pixely * 0.3), color);
		pixelx += deltax;
		pixely += deltay;
		--pixels;
	}
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
