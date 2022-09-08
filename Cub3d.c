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
	float x = p.x;
	float y = p.y;
	draw_line(p.i.mlx, p.i.win, x, y, \
	x + cos(p.rotationAngle) * 40, y + sin(p.rotationAngle) * 40,
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
float cast_ray_horz(t_player *player, float angle)
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
	float x_intercept = player->x + ((y_intercept - player->y) / tan(angle));
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
	draw_line(player->i.mlx,player->i.win,player->x, player->y, Wallhitx, Wallhity, 16711680);
	return (distance(player, Wallhitx, Wallhity));
}

float cast_ray_vert(t_player *player, float angle)
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
	if((isRayDown && y_step > 0) || (isRayUp && y_step < 0))
		y_step *= -1;
	float Wallvertx = x_intercept;
	float Wallverty = y_intercept;
	float Wallhitx = 0;
	float Wallhity = 0;
	while (Wallvertx >= 0 && Wallvertx <= 15 * 50 && Wallverty >= 0 && Wallverty <= 8 * 50)
	{
		float tocheckx = Wallvertx;
		float tochecky = Wallverty;
		if(isRayUp)
			tochecky = Wallverty + 1;
		if(isWall(tocheckx, tochecky))
		{
			Wallhitx = Wallvertx;
			Wallhity = Wallverty;
			break;
		}
		else
		{
			Wallvertx += x_step;
			Wallverty += y_step;
		}
	}
	return (distance(player, Wallhitx, Wallhity));
}
void render_rays(t_player *player)
{
	float angle = player->rotationAngle - (FOV / 2);
	int rayid = 0;
	while (rayid < 500)
	{
		float horz = cast_ray_horz(player, angle);
		float vert = cast_ray_vert(player, angle);
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
	player.rotationAngle = M_PI / 2;
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