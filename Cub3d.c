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

void	ft_draw_elem(int x, int y, void *path, t_img i)
{
	int	height;
	int	width;

	i.img = mlx_xpm_file_to_image(i.mlx, path, &width, &height);
	mlx_put_image_to_window (i.mlx, i.win, i.img, (x * 50) * MINIMAP_FACTOR, (y * 50) * MINIMAP_FACTOR);
}
void draw_line(void *mlx, void *win, float beginX, float beginY, float endX, float endY, int color)
{
	double deltaX = endX - beginX;
	double deltaY = endY - beginY;
	int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;
	double pixelX = beginX;
	double pixelY = beginY;
	while (pixels)
	{
	    mlx_pixel_put(mlx, win, pixelX * MINIMAP_FACTOR, pixelY * MINIMAP_FACTOR, color);
	    pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}

void render_map(t_player p)
{
	int y = 0;
	int x;
	while (y < 8)
	{
		x = 0;
		while (x < 15)
		{
			if(arr[y][x] == '1')
				ft_draw_elem(x, y, "black.xpm", p.i);
			else
				ft_draw_elem(x, y, "pure_white.xpm", p.i);
			x++;
		}
		y++;
	}
}
void render_player(t_player p)
{
	float start_x = p.x;
	float start_y = p.y;
	float end_x = cos(p.rotationAngle) * 90;
	float end_y = sin(p.rotationAngle) * 90;
	draw_line(p.i.mlx, p.i.win, (start_x), start_y, \
	(start_x + end_x), (start_y + end_y),
	16711680);
}
int isWall(float a, float b)
{
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

void put_stripin3D(t_player *player, int project_height, int index)
{
	int color = 15000678;
    t_data  img;
	int x;
	int y;

	x = 0;
	y = 0;
    img.img = mlx_new_image(player->i.mlx, 1, project_height);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
    	&img.endian);
	while (y < project_height)
		my_mlx_pixel_put(&img, x, y++, color);
	mlx_put_image_to_window(player->i.mlx, player->i.win, img.img, index, (750 / 2) - (project_height / 2));
	mlx_destroy_image(player->i.mlx, img.img);
}
void render_3D(t_player *player, t_ray *rays)
{
	int i = 0;
	int distance_toprojection;
	int projection_wall_height;

	distance_toprojection = 375 / tan((FOV / 2));
	while (i < 750)
	{
		projection_wall_height = (TILE_SIZE / (rays[i].distance)) * distance_toprojection;
		put_stripin3D(player, projection_wall_height, i);
		i++;
	}
}

void render_minimap(t_player *player, t_ray *rays)
{
	int i = 0;
	while (i < 750)
	{
		draw_line(player->i.mlx, player->i.win, player->x, player->y, rays[i].Wallhitx, rays[i].Wallhity, 15000678);
		i++;
	}
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
	change_player_status(player);
	render_map(*player);
	get_rays(player, &rays);
	render_3D(player, rays);
	render_minimap(player, rays);
	free(rays);
	player->walkDirection = 0;
	player->turnDirection = 0;
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
	render_map(player);
	render_player(player);
	mlx_key_hook(data.win, next_frame, &player);
	mlx_loop(data.mlx);
}