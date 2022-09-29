#include "../cub3d.h"
#include <stdio.h>

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	ft_draw_elem(int x, int y, t_player *player, int color)
{
	int end_y = ((y * 64) + 64) * MINIMAP_FACTOR  ;
	int end_x = ((x * 64) + 64) * MINIMAP_FACTOR ;
	int start_x;
	int start_y = (y * 64)  * MINIMAP_FACTOR;
	while (start_y < end_y)
	{
		start_x = (x * 64) * MINIMAP_FACTOR ;
		while (start_x < end_x)
		{
			my_mlx_pixel_put(&player->img, (start_x), (start_y), color);
			start_x++;
		}
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
	    my_mlx_pixel_put(&player->img, ((pixelX) * MINIMAP_FACTOR ), ((pixelY) * MINIMAP_FACTOR ), color);
	    pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}

int isWall(float a, float b, t_player *player)
{
	if(a < 0 || a > player->width * TILE_SIZE || b < 0 || b > player->height * TILE_SIZE)
		return 1;
	int wallcheckx = floor(a / 64);
	int wallchecky = floor(b / 64);
	if(wallchecky >= player->height || wallcheckx >= player->width_for_each[wallchecky]
	|| wallchecky < 0 || wallcheckx < 0)
		return (1);
	return (player->data->map[wallchecky][wallcheckx] == '1');
}
void change_player_status(t_player *player)
{
	player->rotationAngle += player->turnDirection * player->turnSpeed;
	float newrotation;
	if(player->rotationAngle != M_PI / 2)
		newrotation = player->rotationAngle - (M_PI / 2);
	float movea;
	float moveb;
	float a = player->x + cos(player->rotationAngle) * (player->walkDirection * player->walkSpeed);
	float b = player->y + sin(player->rotationAngle) * (player->walkDirection * player->walkSpeed);
	if(!isWall(a, b, player))
	{
		player->x = a;
		player->y = b;
	}
	if(player->moveDirection != 0)
	{
		movea = player->x + (15 * player->moveDirection * cos(newrotation));
		moveb = player->y + (15 * player->moveDirection * sin(newrotation));
		if(!isWall(movea, moveb, player))
		{
			player->x = movea;
			player->y = moveb;
		}
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
	if(ray[rayid].isRay_up && !was_vertical)
		ray[rayid].ray_content = 1;
	else if(ray[rayid].isRay_down && !was_vertical)
		ray[rayid].ray_content = 2;
	else if(ray[rayid].isRay_right && was_vertical)
		ray[rayid].ray_content = 3;
	else if(ray[rayid].isRay_left && was_vertical)
		ray[rayid].ray_content = 4;
}

void render_map(t_player *p)
{
	int y = 0;
	int x;
	while (p->data->map[y])
	{
		x = 0;
		while (p->data->map[y][x])
		{
			if(p->data->map[y][x] == '1')
				ft_draw_elem(x, y, p, 0x00FF00);
			else if (p->data->map[y][x] == ' ')
			{
				x++;
				continue;
			}
			else if(p->data->map[y][x] == 'N'
			|| p->data->map[y][x] == 'S'
			|| p->data->map[y][x] == 'W'
			|| p->data->map[y][x] == 'E')
				ft_draw_elem(x, y, p, 0x30FF0000);
			else
				ft_draw_elem(x, y, p, 0x0000FF);
			x++;
		}
		y++;
	}
}

void render_minimap(t_player *player)
{
	int i = 0;
	render_map(player);
	while (i < 1800)
	{
		draw_line(player, player->ray[i].Wallhitx, player->ray[i].Wallhity, 0xCFCDFF);
		i++;
	}
}
void cast_ray(t_player *player, float angle, int rayid, int fd)
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
	while (WallHorzx >= 0 && WallHorzx <= player->width * TILE_SIZE && WallHorzy >= 0 && WallHorzy <= player->height * TILE_SIZE)
	{
		float horz_tocheckx = WallHorzx;
		float horz_tochecky = WallHorzy;
		if(isRayUp)
			horz_tochecky -= 1;
		if(isWall(horz_tocheckx, horz_tochecky, player))
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
	while (vert_WallHorzx >= 0 && vert_WallHorzx <= player->width * TILE_SIZE && vert_WallHorzy >= 0 && vert_WallHorzy <= player->height * TILE_SIZE)
	{
		float vert_tocheckx = vert_WallHorzx;
		float vert_tochecky = vert_WallHorzy;
		if(isRayleft)
			vert_tocheckx -= 1;
		if(isWall(vert_tocheckx, vert_tochecky, player))
		{
			found_vert = TRUE;
			if(vert_WallHorzx <= 1)
				vert_Wallhitx = vert_WallHorzx - 64;
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
		fill_data(player->ray, angle, horz_Wallhitx, horz_Wallhity, 0, isRayleft, isRayright,\
isRayUp, isRayDown, rayid, distance_check1);
	}
	else if(distance_check2 < distance_check1)
	{
		fill_data(player->ray, angle, vert_Wallhitx, vert_Wallhity, 1, isRayleft, isRayright,\
isRayUp, isRayDown, rayid, distance_check2);
	}
}
void get_rays(t_player *player)
{
	int rayid = 0;
	float projec = (1200 / tan((FOV / 2)));
	int fd = open("k.c", O_RDWR | O_CREAT);
	while (rayid < 1800)
	{
		float angle2 = player->rotationAngle + atan((rayid - (1800 / 2)) / projec);
		cast_ray(player, angle2, rayid, fd);
		rayid++;
	}
}

unsigned int get_color(int y, int x, t_data *img)
{
	char *dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

void put_stripin3D(t_player *player, float project_height, int index, int fd)
{
	int put_y;
	int y;
	int ceil_y;
	int floor_y;
	int stock;
	int Xoffset;
	int Y_loop;
	if(player->ray[index].was_hit_vertical)
		Xoffset = (int)(player->ray[index].Wallhity) % TILE_SIZE;
	else
		Xoffset = (int)(player->ray[index].Wallhitx) % TILE_SIZE;
	put_y = (int) ((1200 / 2) - (project_height / 2));
	if(put_y < 0)
		put_y = 0;
	ceil_y = 0;
	floor_y = put_y + project_height;
	Y_loop = put_y;
	if(put_y > 0)
	{
		while (ceil_y < put_y)
			my_mlx_pixel_put(&player->img, index, ceil_y++, player->data->CEILING_COLOR);
		while (ceil_y < floor_y)
		{
			int Yoffset = ((Y_loop + ((project_height / 2) - (1200 / 2))) * ((float) (TILE_SIZE / project_height)));
			if(Yoffset < 0)
				Yoffset = 0;
			if(Yoffset > 63 || Xoffset > 63)
			{
				dprintf(fd, "ray id %d : distance %f : down %d : left %d : right %d : up %d : ray angle %f : ray cont %d : hitx %f : hity %f : vertical %d\n", index,player->ray[index].distance, player->ray[index].isRay_down, player->ray[index].isRay_left, player->ray[index].isRay_right,\
				player->ray[index].isRay_up, player->ray[index].ray_angle, player->ray[index].ray_content, player->ray[index].Wallhitx, player->ray[index].Wallhity,\
				player->ray[index].was_hit_vertical);
				dprintf(fd, "Yoffsey %d and Xoffset %d\n", Yoffset, Xoffset);
				break ;
			}
			if(player->ray[index].ray_content == 1)
				my_mlx_pixel_put(&player->img, index, ceil_y++, get_color(Yoffset, Xoffset, &player->img1));
			else if(player->ray[index].ray_content == 2)
				my_mlx_pixel_put(&player->img, index, ceil_y++, get_color(Yoffset, Xoffset, &player->img2));
			else if(player->ray[index].ray_content == 3)
				my_mlx_pixel_put(&player->img, index, ceil_y++, get_color(Yoffset, Xoffset, &player->img3));
			else if(player->ray[index].ray_content == 4)
				my_mlx_pixel_put(&player->img, index, ceil_y++, get_color(Yoffset, Xoffset, &player->img4));
			Y_loop++;
		}
		while (floor_y < 1200)
			my_mlx_pixel_put(&player->img, index, floor_y++, player->data->FLOOR_COLOR);
	}
	else
	{
		y = 0;
		while (y < 1200)
		{
			int Yoffset = (Y_loop + ((project_height / 2) - (1200 / 2))) * ((float) (TILE_SIZE / project_height));
			if(Yoffset < 0)
				Yoffset = 0;
			if(Yoffset > 63 || Xoffset > 63)
			{
				dprintf(fd, "ray id %d : distance %f : down %d : left %d : right %d : up %d : ray angle %f : ray cont %d : hitx %f : hity %f : vertical %d\n", index,player->ray[index].distance, player->ray[index].isRay_down, player->ray[index].isRay_left, player->ray[index].isRay_right,\
				player->ray[index].isRay_up, player->ray[index].ray_angle, player->ray[index].ray_content, player->ray[index].Wallhitx, player->ray[index].Wallhity,\
				player->ray[index].was_hit_vertical);
				dprintf(fd, "Yoffsey %d and Xoffset %d\n", Yoffset, Xoffset);
				break ;
			}
			if(player->ray[index].ray_content == 1)
				my_mlx_pixel_put(&player->img, index, y++, get_color(Yoffset, Xoffset, &player->img1));
			else if(player->ray[index].ray_content == 2)
				my_mlx_pixel_put(&player->img, index, y++, get_color(Yoffset, Xoffset, &player->img2));
			else if(player->ray[index].ray_content == 3)
				my_mlx_pixel_put(&player->img, index, y++, get_color(Yoffset, Xoffset, &player->img3));
			else if(player->ray[index].ray_content == 4)
				my_mlx_pixel_put(&player->img, index, y++, get_color(Yoffset, Xoffset, &player->img4));
			Y_loop++;
		}
	}
}
void render_3D(t_player *player)
{
	int i = 0;
	float distance_toprojection;
	float projection_wall_height;
	distance_toprojection = 1200 / tan((FOV / 2));
	int fd = open("x.c", O_RDWR | O_CREAT);
	while (i < 1800)
	{
		projection_wall_height = (TILE_SIZE / (player->ray[i].distance * cos(player->ray[i].ray_angle - player->rotationAngle))) * distance_toprojection;
		put_stripin3D(player, projection_wall_height, i, fd);
		i++;
	}
}

int next_frame(int key, t_player *player)
{
	if(key == 53)
	{
		free(player->ray);
		free(player->width_for_each);
		exit(1);
	}
	if (key == 13 || key == 126)
		player->walkDirection = 1;
	if (key == 123)
		player->turnDirection = -1;
	if(key == 0)
		player->moveDirection = 1;
	if(key == 2)
		player->moveDirection = -1;
	if (key == 1 || key == 125)
		player->walkDirection = -1;
	if (key == 124)
		player->turnDirection = 1;
	mlx_clear_window(player->i.mlx, player->i.win);
	change_player_status(player);
	get_rays(player);
	render_3D(player);
	// render_minimap(player);
	mlx_put_image_to_window(player->i.mlx, player->i.win, player->img.img, 0, 0);
	player->walkDirection = 0;
	player->turnDirection = 0;
	player->moveDirection = 0;
	return 0;
}
int stop(void)
{
	return 0;
}

float get_init_pos(t_directions *path)
{
	if(path->START_POSITION == 'N')
		return ((3 * M_PI) / 2);
	if(path->START_POSITION == 'W')
		return (M_PI);
	if(path->START_POSITION == 'S')
		return ((M_PI / 2));
	if(path->START_POSITION == 'E')
		return (0);
	return 0;
}

int get_width(t_directions *path)
{
	int max = ft_strlen(path->map[0]);
	int len;
	int i = 1;
	while (path->map[i])
	{
		len = ft_strlen(path->map[i++]);
		if(len > max)
			max = len;
	}
	return max;
}
int get_height(t_directions *path)
{
	int i = 0;
	while (path->map[i])
		i++;
	return (i);
}

int *get_widths(t_directions *path, int height)
{
	int len;
	int *wid_arr;

	wid_arr = malloc(sizeof(int) * height);
	len = 0;
	while (len < height)
	{
		int width = 0;
		while (path->map[len][width])
			width++;
		wid_arr[len++] = width;
	}
	return (wid_arr);
}
int close_win(t_player *player)
{
	free(player->width_for_each);
	free(player->ray);
	exit(1);
}
void start_game(t_directions *path)
{
	t_ray *rays;
	t_img data;
	t_player player;
	int width;
	int height;

	width = get_width(path);
	height = get_height(path);
	player.x = (path->PLAYER_X + 0.5) * TILE_SIZE;
	player.y = (path->PLAYER_Y + 0.5) * TILE_SIZE;
	player.height = height;
	player.width = width;
	player.width_for_each = get_widths(path, height);
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.tab_press  = 0;
	player.rotationAngle = get_init_pos(path);
	player.walkSpeed = 0.60 * TILE_SIZE;
	player.turnSpeed = 10 * (M_PI / 180);
	data.mlx = mlx_init();
	data.win = mlx_new_window(data.mlx, 1800, 1200, "Cub3d");
	player.ray = malloc(1800 * sizeof(t_ray));
	player.i = data;
	player.img.img = mlx_new_image(data.mlx, 1800, 1200);
	player.img.addr = mlx_get_data_addr(player.img.img, &player.img.bits_per_pixel, &player.img.line_length,
    	&player.img.endian);
	player.img1.img = mlx_xpm_file_to_image(data.mlx, "path_to_the_east_texture", &player.pic_width,&player.pic_height);
	player.img1.addr = mlx_get_data_addr(player.img1.img, &player.img1.bits_per_pixel, &player.img1.line_length,
    	&player.img1.endian);
	player.img2.img = mlx_xpm_file_to_image(data.mlx, "path_to_the_north_texture", &player.pic_width,&player.pic_height);
	player.img2.addr = mlx_get_data_addr(player.img2.img, &player.img2.bits_per_pixel, &player.img2.line_length,
		 &player.img2.endian);
	player.img3.img = mlx_xpm_file_to_image(data.mlx, "path_to_the_west_texture", &player.pic_width,&player.pic_height);
	player.img3.addr = mlx_get_data_addr(player.img3.img, &player.img3.bits_per_pixel, &player.img3.line_length,
    	&player.img3.endian);
	player.img4.img = mlx_xpm_file_to_image(data.mlx, "path_to_the_south_texture", &player.pic_width,&player.pic_height);
	player.img4.addr = mlx_get_data_addr(player.img4.img, &player.img4.bits_per_pixel, &player.img4.line_length,
    	&player.img4.endian);
	player.data = path;
	get_rays(&player);
	render_3D(&player);
	// render_minimap(&player);
	mlx_put_image_to_window(player.i.mlx, player.i.win, player.img.img, 0, 0);
	mlx_hook(data.win, 2, 0, next_frame, &player);
	mlx_hook(data.win, 3, 0, stop, NULL);
	mlx_hook(data.win, 17, (1L<<15) , close_win, &player);
	mlx_loop(data.mlx);
}