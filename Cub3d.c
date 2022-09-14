#include "cub3d.h"
#include <stdio.h>
char arr[8][15] = 
{
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
	{'1','0','0','0','0','0','0','0','0','0','1','1','0','0','1'},
	{'1','0','1','1','0','0','0','0','0','1','1','1','1','0','1'},
	{'1','1','0','0','0','0','0','0','0','0','1','1','0','1','1'},
	{'1','0','0','0','0','1','0','0','0','0','1','1','1','1','1'},
	{'1','0','0','0','0','0','0','0','0','0','1','1','0','0','1'},
	{'1','0','1','1','0','0','0','0','0','1','1','1','1','0','1'},
	{'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
} ;

void	ft_draw_elem(int x, int y, void *path, t_img i)
{
	int	height;
	int	width;

	i.img = mlx_xpm_file_to_image(i.mlx, path, &width, &height);
	mlx_put_image_to_window (i.mlx, i.win, i.img, x * 50, y * 50);
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
	    mlx_pixel_put(mlx, win, pixelX, pixelY, color);
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
	draw_line(p.i.mlx, p.i.win, start_x, start_y, \
	start_x + end_x, start_y + end_y,
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
void cast_ray_horz(t_player *player, float angle, float *distance_check, t_ray *ray)
{
	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	int isRayDown = (angle > 0 && angle < M_PI);
	int isRayUp = !isRayDown;
	int isRayright = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	int isRayleft = !isRayright;
	float y_intercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
	if(isRayDown)
		y_intercept += TILE_SIZE;
	float x_intercept = player->x + (y_intercept - player->y) / tan(angle);
	float y_step = TILE_SIZE;
	if(isRayUp)
		y_step *= -1;
	float x_step = y_step / tan(angle);
	if((isRayleft && x_step > 0) || (isRayright && x_step < 0))
		x_step *= -1;
	float WallHorzx = x_intercept;
	float WallHorzy = y_intercept;
	float Wallhitx = 0;
	float Wallhity = 0;
	while (WallHorzx >= 0 && WallHorzx <= 15 * 50 && WallHorzy >= 0 && WallHorzy <= 8 * 50)
	{
		float tocheckx = WallHorzx;
		float tochecky = WallHorzy;
		if(isRayUp)
			tochecky = WallHorzy + 1;
		if(isWall(tocheckx, tochecky))
		{
			Wallhitx = WallHorzx;
			if(WallHorzy <= 1)
				Wallhity = WallHorzy + 50;
			else
				Wallhity = WallHorzy;
			break;
		}
		else
		{
			WallHorzx += x_step;
			WallHorzy += y_step;
		}
	}
	*distance_check = distance(player, Wallhitx, Wallhity);
	ray->Wallhitx = Wallhitx;
	ray->Wallhity = Wallhity;
}

void cast_ray_vert(t_player *player, float angle, float *distance_check, t_ray *ray)
{
	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	int isRayDown = (angle > 0 && angle < M_PI);
	int isRayUp = !isRayDown;
	int isRayright = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	int isRayleft = !isRayright;
	float x_intercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
	if(isRayright)
		x_intercept += TILE_SIZE;
	float y_intercept = player->y + ((x_intercept - player->x) * tan(angle));
	float x_step = TILE_SIZE;
	if(isRayleft)
		x_step *= -1;
	float y_step = x_step * tan(angle);
	if((isRayUp && y_step > 0) || (isRayDown && y_step < 0))
		y_step *= -1;
	float WallHorzx = x_intercept;
	float WallHorzy = y_intercept;
	float Wallhitx = 0;
	float Wallhity = 0;
	while (WallHorzx >= 0 && WallHorzx <= 15 * TILE_SIZE && WallHorzy >= 0 && WallHorzy <= 8 * TILE_SIZE)
	{
		float tocheckx = WallHorzx;
		float tochecky = WallHorzy;
		if(isRayleft)
			tocheckx = WallHorzx - 1;
		if(isWall(tocheckx, tochecky))
		{
			Wallhitx = WallHorzx;
			Wallhity = WallHorzy;
			break;
		}
		else
		{
			WallHorzx += x_step;
			WallHorzy += y_step;
		}
	}
	draw_line(player->i.mlx, player->i.win, player->x, player->y, Wallhitx, Wallhity, 16711680);
}

void cast_ray(t_player *player, float angle, t_ray ray)
{
	float distance_check1;
	float distance_check2;

	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	int isRayDown = (angle > 0 && angle < M_PI);
	int isRayUp = !isRayDown;
	int isRayright = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	int isRayleft = !isRayright;
	// HORIZONTAL
	float horz_y_intercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
	if(isRayDown)
		horz_y_intercept += TILE_SIZE;
	float horz_x_intercept = player->x + (horz_y_intercept - player->y) / tan(angle);
	float horz_y_step = TILE_SIZE;
	if(isRayUp)
		horz_y_step *= -1;
	float horz_x_step = horz_y_step / tan(angle);
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
			horz_tochecky = WallHorzy + 1;
		if(isWall(horz_tocheckx, horz_tochecky))
		{
			horz_Wallhitx = WallHorzx;
			if(WallHorzy <= 1)
				horz_Wallhity = WallHorzy + 50;
			else
				horz_Wallhity = WallHorzy;
			break;
		}
		else
		{
			WallHorzx += horz_x_step;
			WallHorzy += horz_y_step;
		}
	}
	distance_check1 = distance(player, horz_Wallhitx, horz_Wallhity);
	// VERTICAL
	float vert_x_intercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
	if(isRayright)
		vert_x_intercept += TILE_SIZE;
	float vert_y_intercept = player->y + ((vert_x_intercept - player->x) * tan(angle));
	float vert_x_step = TILE_SIZE;
	if(isRayleft)
		vert_x_step *= -1;
	float vert_y_step = vert_x_step * tan(angle);
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
			vert_tocheckx = vert_WallHorzx - 1;
		if(isWall(vert_tocheckx, vert_tochecky))
		{
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
	distance_check2 = distance(player, vert_Wallhitx, vert_Wallhity);
	printf("%f and %f\n", distance_check1, distance_check2);
	if(distance_check1 > distance_check2)
		draw_line(player->i.mlx, player->i.win, player->x, player->y, horz_Wallhitx, horz_Wallhity, 16711680);
	else
		draw_line(player->i.mlx, player->i.win, player->x, player->y, vert_Wallhitx, vert_Wallhity, 16711680);
}
void render_rays(t_player *player)
{
	float angle = player->rotationAngle - (FOV / 2);
	t_ray rays[500];

	int rayid = 0;
	while (rayid < 500)
	{
		cast_ray(player, angle, rays[rayid]);
		angle += (FOV / 500);
		rayid++;
	}
}
int next_frame(int key, t_player *player)
{
	if (key == 13 || key == 126)
		player->walkDirection = 1;
	if (key == 0 || key == 123)
		player->turnDirection = -1;
	if (key == 1 || key == 125)
		player->walkDirection = -1;
	if (key == 2 || key == 124)
		player->turnDirection = 1;
	change_player_status(player);
	render_map(*player);
	render_player(*player);
	render_rays(player);
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
	player.rotationAngle = M_PI;
	player.walkSpeed = 0.15 * TILE_SIZE;
	player.turnSpeed = 10 * (M_PI / 180);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, 15 * 50, 8 * 50, "Cub3d");
	player.i = data;
	render_map(player);
	render_player(player);
	mlx_key_hook(data.win, next_frame, &player);
	mlx_loop(data.mlx);
}