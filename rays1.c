/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:30:33 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/01 21:40:07 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	set_distance(t_ray_steps *data, t_player *player)
{
	if (data->found_wall)
		data->distance = distance(player, data->Wall_x, data->Wall_y);
	else
		data->distance = MAX_INT;
}

void	horz_distance(t_ray_steps *data, t_player *player)
{
	float	horz_tochecky;

	data->found_wall = FALSE;
	while (data->x_intercept >= 0
		&& data->x_intercept <= player->width * TILE_SIZE
		&& data->y_intercept >= 0
		&& data->y_intercept <= player->height * TILE_SIZE)
	{
		horz_tochecky = data->y_intercept;
		if (data->ray_up)
			horz_tochecky -= 1;
		if (iswall(data->x_intercept, horz_tochecky, player))
		{
			data->Wall_x = data->x_intercept;
			data->Wall_y = data->y_intercept;
			data->found_wall = TRUE;
			break ;
		}
		data->x_intercept += data->xstep;
		data->y_intercept += data->ystep;
	}
	set_distance(data, player);
}

void	vert_distance(t_ray_steps *data, t_player *player)
{
	float	vert_tocheckx;

	data->found_wall = FALSE;
	while (data->x_intercept >= 0
		&& data->x_intercept <= player->width * TILE_SIZE
		&& data->y_intercept >= 0
		&& data->y_intercept <= player->height * TILE_SIZE)
	{
		vert_tocheckx = data->x_intercept;
		if (data->ray_left)
			vert_tocheckx -= 1;
		if (iswall(vert_tocheckx, data->y_intercept, player))
		{
			data->found_wall = TRUE;
			if (data->x_intercept <= 1)
				data->Wall_x = data->x_intercept - 64;
			else
				data->Wall_x = data->x_intercept;
			data->Wall_y = data->y_intercept;
			break ;
		}
		data->x_intercept += data->xstep;
		data->y_intercept += data->ystep;
	}
	set_distance(data, player);
}

void	cast_ray(t_player *player, float angle)
{
	t_ray_steps	horz_result;
	t_ray_steps	vert_result;

	angle = remainder(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	horz_result = get_horz_steps(player, angle);
	vert_result = get_vert_steps(player, angle);
	horz_distance(&horz_result, player);
	vert_distance(&vert_result, player);
	if (horz_result.distance < vert_result.distance)
		fill_data(player->ray, angle, horz_result, 0);
	else if (vert_result.distance < horz_result.distance)
		fill_data(player->ray, angle, vert_result, 1);
}

void	get_rays(t_player *player)
{
	int		rayid;
	float	projec;
	float	angle;

	rayid = 0;
	projec = (900 / tan((FOV / 2)));
	while (rayid < 1500)
	{
		angle = player->rotationAngle + atan((rayid - (1500 / 2)) / projec);
		cast_ray(player, angle);
		rayid++;
	}
}
