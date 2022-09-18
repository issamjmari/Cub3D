#include "cub3d.h"
#include <stdio.h>
char arr[8][15] = 
{
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
	{'1','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','1','0','0','0','0','0','0','1','0','0','1'},
	{'1','0','1','0','0','0','0','0','0','0','0','0','0','0','1'},
	{'1','0','0','0','0','0','0','1','1','0','0','0','0','0','1'},
	{'1','0','1','0','1','0','0','0','0','0','0','1','0','0','1'},
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
} ;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	ft_draw_elem(int x, int y, t_img i, t_data *data, int color)
{
	int end_y = ((y * 50) + 50) * MINIMAP_FACTOR;
	int end_x = ((x * 50) + 50) * MINIMAP_FACTOR;
	int start_x;
	int start_y = (y * 50) * MINIMAP_FACTOR;
	while (start_y < end_y)
	{
		start_x = (x * 50) * MINIMAP_FACTOR;
		while (start_x < end_x)
			my_mlx_pixel_put(data, start_x++, start_y, color);
		start_y++;
	}
}
void draw_line(t_player *player, float endX, float endY, int color)
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
	    my_mlx_pixel_put(&player->img, pixelX * MINIMAP_FACTOR, pixelY * MINIMAP_FACTOR, color);
	    pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}

void render_map(t_player *p)
{
	int y = 0;
	int x;
	while (y < 8)
	{
		x = 0;
		while (x < 15)
		{
			if(arr[y][x] == '1')
				ft_draw_elem(x, y, p->i, &p->img, 0x00FF00);
			else
				ft_draw_elem(x, y, p->i, &p->img, 0x0000FF);
			x++;
		}
		y++;
	}
}

int isWall(float a, float b)
{
	if(a < 0 || a > 750 || b < 0 || b > 750)
		return 1;
	int wallcheckx =  floor(a / 50);
	int wallchecky = floor(b / 50);
	return (arr[wallchecky][wallcheckx] == '1');
}
void change_player_status(t_player *player)
{
	player->rotationAngle += player->turnDirection * player->turnSpeed;
	float a = player->x + cos(player->rotationAngle) * (player->walkDirection * player->walkSpeed);
	float b = player->y + sin(player->rotationAngle) * (player->walkDirection * player->walkSpeed);
	if(!isWall(a, b))
	{
		player->x = a;
		player->y = b;
	}
}

float distance(t_player *p, float Wallx, float Wally)
{
	return (sqrt((Wallx - p->x) * (Wallx - p->x) + (Wally - p->y) * (Wally - p->y)));
}
void fill_data(t_ray *ray, float angle, float hitx, float hity,\
int was_vertical, int isRayleft, int isRayright, int isRayup, int isRaydown, int rayid,\
float distance)
{
	ray[rayid].distance = distance;
	ray[rayid].ray_angle = angle;
	ray[rayid].Wallhitx = hitx;
	ray[rayid].Wallhity = hity;
	ray[rayid].was_hit_vertical = was_vertical;
	ray[rayid].isRay_down = isRaydown;
	ray[rayid].isRay_up = isRayup;
	ray[rayid].isRay_left = isRayleft;
	ray[rayid].isRay_right = isRayright;
}
void cast_ray(t_player *player, float angle, t_ray *ray, int rayid)
{
	float distance_check1;
	float distance_check2;
	int found_horz = FALSE;
	int found_vert = FALSE;

	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	int isRayDown = (angle > 0 && angle < M_PI);
	int isRayUp = !isRayDown;
	int isRayright = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	int isRayleft = !isRayright;
	// // HORIZONTAL
	float horz_y_intercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
	if(isRayDown)
		horz_y_intercept += TILE_SIZE;
	float horz_x_intercept = player->x + (horz_y_intercept - player->y) / tan(angle);
	float horz_y_step = TILE_SIZE;
	if(isRayUp)
		horz_y_step *= -1;
	float horz_x_step = TILE_SIZE / tan(angle);
	if((isRayleft && horz_x_step > 0) || (isRayright && horz_x_step < 0))
		horz_x_step *= -1;
	float WallHorzx = horz_x_intercept;
	float WallHorzy = horz_y_intercept;
	float horz_Wallhitx = 0;
	float horz_Wallhity = 0;
	while (WallHorzx >= 0 && WallHorzx <= 15 * 50 && WallHorzy >= 0 && WallHorzy <= 8 * 50)
	{
		float horz_tocheckx = WallHorzx;
		float horz_tochecky = WallHorzy;
		if(isRayUp)
			horz_tochecky -= 1;
		if(isWall(horz_tocheckx, horz_tochecky))
		{
			horz_Wallhitx = WallHorzx;
			horz_Wallhity = WallHorzy;
			found_horz = TRUE;
			break;
		}
		else
		{
			WallHorzx += horz_x_step;
			WallHorzy += horz_y_step;
		}
	}
	if(found_horz)
		distance_check1 = distance(player, horz_Wallhitx, horz_Wallhity);
	else
		distance_check1 = MAX_INT;
	// VERTICAL
	float vert_x_intercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
	if(isRayright)
		vert_x_intercept += TILE_SIZE;
	float vert_y_intercept = player->y + ((vert_x_intercept - player->x) * tan(angle));
	float vert_x_step = TILE_SIZE;
	if(isRayleft)
		vert_x_step *= -1;
	float vert_y_step = TILE_SIZE * tan(angle);
	if((isRayUp && vert_y_step > 0) || (isRayDown && vert_y_step < 0))
		vert_y_step *= -1;
	float vert_WallHorzx = vert_x_intercept;
	float vert_WallHorzy = vert_y_intercept;
	float vert_Wallhitx = 0;
	float vert_Wallhity = 0;
	while (vert_WallHorzx >= 0 && vert_WallHorzx <= 15 * TILE_SIZE && vert_WallHorzy >= 0 && vert_WallHorzy <= 8 * TILE_SIZE)
	{
		float vert_tocheckx = vert_WallHorzx;
		float vert_tochecky = vert_WallHorzy;
		if(isRayleft)
			vert_tocheckx -= 1;
		if(isWall(vert_tocheckx, vert_tochecky))
		{
			found_vert = TRUE;
			if(vert_WallHorzx <= 1)
				vert_Wallhitx = vert_WallHorzx - 50;
			else
				vert_Wallhitx = vert_WallHorzx;
			vert_Wallhity = vert_WallHorzy;
			break;
		}
		else
		{
			vert_WallHorzx += vert_x_step;
			vert_WallHorzy += vert_y_step;
		}
	}
	if(found_vert)
		distance_check2 = distance(player, vert_Wallhitx, vert_Wallhity);
	else
		distance_check2 = MAX_INT;
	if(distance_check1 < distance_check2)
	{
		fill_data(ray, angle, WallHorzx, WallHorzy, 0, isRayleft, isRayright,\
isRayUp, isRayDown, rayid, distance_check1);
	}
	else
	{
		fill_data(ray, angle, vert_Wallhitx, vert_Wallhity, 1, isRayleft, isRayright,\
isRayUp, isRayDown, rayid, distance_check2);
	}
}
void get_rays(t_player *player, t_ray **rays)
{
	float angle = player->rotationAngle - (FOV / 2);
	int rayid = 0;
	while (rayid < 750)
	{
		cast_ray(player, angle, *rays, rayid);
		angle += (FOV / (750));
		rayid++;
	}
}

void put_stripin3D(t_player *player, int project_height, int index, int color)
{
    t_data  img;
	t_data ceil;
	t_data floor;
	int put_y;
	int y;
	int ceil_y;

	put_y = (400 / 2) - (project_height / 2);
	if(put_y < 0)
		put_y = 0;
	y = 0;
	ceil_y = 0;
    img.img = mlx_new_image(player->i.mlx, 1, project_height);
	ceil.img = mlx_new_image(player->i.mlx, 1, (put_y) - 1);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
    	&img.endian);
	// ceil.addr = mlx_get_data_addr(ceil.img, &ceil.bits_per_pixel, &ceil.line_length,
    // 	&ceil.endian);
	// while (ceil_y < (put_y) - 1)
	// 	my_mlx_pixel_put(&img, 0, ceil_y++, 0xFFFFFF);
	while (y < project_height)
		my_mlx_pixel_put(&img, 0, y++, color);
	mlx_put_image_to_window(player->i.mlx, player->i.win, img.img, index, put_y);
	// mlx_put_image_to_window(player->i.mlx, player->i.win, img.img, index, 0);
	mlx_destroy_image(player->i.mlx, img.img);
	// mlx_destroy_image(player->i.mlx, ceil.img);
}
void render_3D(t_player *player, t_ray *rays)
{
	int i = 0;
	int distance_toprojection;
	int projection_wall_height;
	distance_toprojection = 375 / tan((FOV / 2));
	while (i < 750)
	{
		projection_wall_height = (TILE_SIZE / (rays[i].distance * cos(rays[i].ray_angle - player->rotationAngle))) * distance_toprojection;
		if(rays[i].was_hit_vertical)
			put_stripin3D(player, projection_wall_height, i, 0x25FF0000);
		else
			put_stripin3D(player, projection_wall_height, i, 0x00FF0000);
		i++;
	}
}

void render_minimap(t_player *player, t_ray *rays)
{
	int i = 0;
	player->img.img = mlx_new_image(player->i.mlx, 750 * MINIMAP_FACTOR, 400 * MINIMAP_FACTOR);
	player->img.addr = mlx_get_data_addr(player->img.img, &player->img.bits_per_pixel, &player->img.line_length,
    	&player->img.endian);
	render_map(player);
	while (i < 750)
	{
		draw_line(player, rays[i].Wallhitx, rays[i].Wallhity, 0xCFCDFF);
		i++;
	}
	mlx_put_image_to_window (player->i.mlx, player->i.win, player->img.img, 0, 0);
}

int next_frame(int key, t_player *player)
{
	t_ray *rays;

	rays = malloc((15 * 50) * sizeof(t_ray));
	if (key == 13 || key == 126)
		player->walkDirection = 1;
	if (key == 0 || key == 123)
		player->turnDirection = -1;
	if (key == 1 || key == 125)
		player->walkDirection = -1;
	if (key == 2 || key == 124)
		player->turnDirection = 1;
	mlx_clear_window(player->i.mlx, player->i.win);
	mlx_destroy_image(player->i.mlx, player->img.img);
	change_player_status(player);
	get_rays(player, &rays);
	render_3D(player, rays);
	render_minimap(player, rays);
	free(rays);
	player->walkDirection = 0;
	player->turnDirection = 0;
	return 0;
}
int stop()
{
	return 0;
}
int main()
{
	t_img data;
	t_player player;
	player.x = (15 / 2) * TILE_SIZE;
	player.y = (8 / 2) * TILE_SIZE;
	player.height = 5;
	player.width = 5;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = (M_PI) / 2;
	player.walkSpeed = 0.30 * TILE_SIZE;
	player.turnSpeed = 10 * (M_PI / 180);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, 15 * 50, 8 * 50, "Cub3d");
	player.i = data;
	player.img.img = mlx_new_image(data.mlx, 750 * MINIMAP_FACTOR, 400 * MINIMAP_FACTOR);
	player.img.addr = mlx_get_data_addr(player.img.img, &player.img.bits_per_pixel, &player.img.line_length,
    	&player.img.endian);
	render_map(&player);
	mlx_put_image_to_window(player.i.mlx, player.i.win, player.img.img, 0, 0);
	mlx_hook(data.win, 2, 0, next_frame, &player);
	mlx_hook(data.win, 3, 0, stop, NULL);
	mlx_loop(data.mlx);
}