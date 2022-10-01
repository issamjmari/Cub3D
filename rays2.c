/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:32:29 by ijmari            #+#    #+#             */
/*   Updated: 2022/10/01 21:40:11 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

float	distance(t_player *p, float Wallx, float Wally)
{
	return (sqrt((Wallx - p->x) * (Wallx - p->x) + \
	(Wally - p->y) * (Wally - p->y)));
}

void	fill_data(t_ray *ray, float angle, t_ray_steps data, int was_vertical)
{
	static int	rayid;

	ray[rayid].distance = data.distance;
	ray[rayid].ray_angle = angle;
	ray[rayid].Wallhitx = data.Wall_x;
	ray[rayid].Wallhity = data.Wall_y;
	ray[rayid].was_hit_vertical = was_vertical;
	ray[rayid].isRay_down = data.ray_down;
	ray[rayid].isRay_up = data.ray_up;
	ray[rayid].isRay_left = data.ray_left;
	ray[rayid].isRay_right = data.ray_right;
	if (ray[rayid].isRay_up && !was_vertical)
		ray[rayid].ray_content = 1;
	else if (ray[rayid].isRay_down && !was_vertical)
		ray[rayid].ray_content = 2;
	else if (ray[rayid].isRay_right && was_vertical)
		ray[rayid].ray_content = 3;
	else if (ray[rayid].isRay_left && was_vertical)
		ray[rayid].ray_content = 4;
	rayid++;
	if (rayid == 1500)
		rayid = 0;
}

t_ray_steps	get_vert_steps(t_player *player, float angle)
{
	t_ray_steps	vert_step;

	angle = remainder(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	vert_step.ray_down = (angle > 0 && angle < M_PI);
	vert_step.ray_up = !vert_step.ray_down;
	vert_step.ray_right = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	vert_step.ray_left = !vert_step.ray_right;
	vert_step.x_intercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
	if (vert_step.ray_right)
		vert_step.x_intercept += TILE_SIZE;
	vert_step.y_intercept = player->y + \
	((vert_step.x_intercept - player->x) * tan(angle));
	vert_step.xstep = TILE_SIZE;
	if (vert_step.ray_left)
		vert_step.xstep *= -1;
	vert_step.ystep = TILE_SIZE * tan(angle);
	if ((vert_step.ray_up && vert_step.ystep > 0)
		|| (vert_step.ray_down && vert_step.ystep < 0))
		vert_step.ystep *= -1;
	return (vert_step);
}

t_ray_steps	get_horz_steps(t_player *player, float angle)
{
	t_ray_steps	horz_step;

	angle = remainder(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	horz_step.ray_down = (angle > 0 && angle < M_PI);
	horz_step.ray_up = !horz_step.ray_down;
	horz_step.ray_right = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	horz_step.ray_left = !horz_step.ray_right;
	horz_step.y_intercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
	if (horz_step.ray_down)
		horz_step.y_intercept += TILE_SIZE;
	horz_step.x_intercept = player->x + \
	(horz_step.y_intercept - player->y) / tan(angle);
	horz_step.ystep = TILE_SIZE;
	if (horz_step.ray_up)
		horz_step.ystep *= -1;
	horz_step.xstep = TILE_SIZE / tan(angle);
	if ((horz_step.ray_left && horz_step.xstep > 0)
		|| (horz_step.ray_right && horz_step.xstep < 0))
		horz_step.xstep *= -1;
	return (horz_step);
}
