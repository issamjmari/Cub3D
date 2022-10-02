/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_image.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:25:30 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/02 15:17:34 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	get_width(t_directions *path)
{
	int	max;
	int	len;
	int	i;

	max = ft_strlen(path->map[0]);
	i = 1;
	while (path->map[i])
	{
		len = ft_strlen(path->map[i++]);
		if (len > max)
			max = len;
	}
	return (max);
}

int	get_height(t_directions *path)
{
	int	i;

	i = 0;
	while (path->map[i])
		i++;
	return (i);
}

int	*get_widths(t_directions *path, int height)
{
	int		len;
	int		*wid_arr;
	int		width;

	wid_arr = malloc(sizeof(int) * height);
	len = 0;
	while (len < height)
	{
		width = 0;
		while (path->map[len][width])
			width++;
		wid_arr[len++] = width;
	}
	return (wid_arr);
}

void	create_images(t_player *player)
{
	player->img.img = mlx_new_image(player->image.mlx, 1500, 900);
	player->img.addr = mlx_get_data_addr(player->img.img, \
	&player->img.bits_per_pixel, &player->img.line_length,
			&player->img.endian);
	player->img1.img = mlx_xpm_file_to_image(player->image.mlx, \
	player->data->NORTH, &player->pic_width, &player->pic_height);
	player->img1.addr = mlx_get_data_addr(player->img1.img, \
	&player->img1.bits_per_pixel, &player->img1.line_length,
			&player->img1.endian);
	player->img2.img = mlx_xpm_file_to_image(player->image.mlx, \
	player->data->SOUTH, &player->pic_width, &player->pic_height);
	player->img2.addr = mlx_get_data_addr(player->img2.img, \
	&player->img2.bits_per_pixel, &player->img2.line_length,
			&player->img2.endian);
	player->img3.img = mlx_xpm_file_to_image(player->image.mlx, \
	player->data->WEST, &player->pic_width, &player->pic_height);
	player->img3.addr = mlx_get_data_addr(player->img3.img, \
	&player->img3.bits_per_pixel, &player->img3.line_length,
			&player->img3.endian);
	player->img4.img = mlx_xpm_file_to_image(player->image.mlx, \
	player->data->EAST, &player->pic_width, &player->pic_height);
	player->img4.addr = mlx_get_data_addr(player->img4.img, \
	&player->img4.bits_per_pixel, &player->img4.line_length,
			&player->img4.endian);
}
