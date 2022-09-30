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
void fill_data(t_ray *ray, float angle, t_ray_steps data, int was_vertical)
{
	static int rayid;

	ray[rayid].distance = data.distance;
	ray[rayid].ray_angle = angle;
	ray[rayid].Wallhitx = data.Wall_x;
	ray[rayid].Wallhity = data.Wall_y;
	ray[rayid].was_hit_vertical = was_vertical;
	ray[rayid].isRay_down = data.ray_down;
	ray[rayid].isRay_up = data.ray_up;
	ray[rayid].isRay_left = data.ray_left;
	ray[rayid].isRay_right = data.ray_right;
	if(ray[rayid].isRay_up && !was_vertical)
		ray[rayid].ray_content = 1;
	else if(ray[rayid].isRay_down && !was_vertical)
		ray[rayid].ray_content = 2;
	else if(ray[rayid].isRay_right && was_vertical)
		ray[rayid].ray_content = 3;
	else if(ray[rayid].isRay_left && was_vertical)
		ray[rayid].ray_content = 4;
	rayid++;
	if(rayid == 1500)
		rayid = 0;
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
	while (i < 1500)
	{
		draw_line(player, player->ray[i].Wallhitx, player->ray[i].Wallhity, 0xCFCDFF);
		i++;
	}
}

t_ray_steps get_vert_steps(t_player *player, float angle)
{
	t_ray_steps vert_step;

	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	vert_step.ray_down = (angle > 0 && angle < M_PI);
	vert_step.ray_up = !vert_step.ray_down;
	vert_step.ray_right = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	vert_step.ray_left = !vert_step.ray_right;
	vert_step.x_intercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
	if(vert_step.ray_right)
		vert_step.x_intercept += TILE_SIZE;
	vert_step.y_intercept = player->y + ((vert_step.x_intercept - player->x) * tan(angle));
	vert_step.xstep = TILE_SIZE;
	if(vert_step.ray_left)
		vert_step.xstep *= -1;
	vert_step.ystep = TILE_SIZE * tan(angle);
	if((vert_step.ray_up && vert_step.ystep > 0) || (vert_step.ray_down && vert_step.ystep < 0))
		vert_step.ystep *= -1;
	return (vert_step);
}
t_ray_steps get_horz_steps(t_player *player, float angle)
{
	t_ray_steps horz_step;

	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	horz_step.ray_down = (angle > 0 && angle < M_PI);
	horz_step.ray_up = !horz_step.ray_down;
	horz_step.ray_right = (angle < 0.5 * M_PI || angle > 1.5 * M_PI);
	horz_step.ray_left = !horz_step.ray_right;
	horz_step.y_intercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
	if(horz_step.ray_down)
		horz_step.y_intercept += TILE_SIZE;
	horz_step.x_intercept = player->x + (horz_step.y_intercept - player->y) / tan(angle);
	horz_step.ystep = TILE_SIZE;
	if(horz_step.ray_up)
		horz_step.ystep *= -1;
	horz_step.xstep = TILE_SIZE / tan(angle);
	if((horz_step.ray_left && horz_step.xstep > 0) || (horz_step.ray_right && horz_step.xstep < 0))
		horz_step.xstep *= -1;
	return (horz_step);
}

void	horz_distance(t_ray_steps *data, t_player *player, float angle)
{
	float WallHorzx;
	float WallHorzy;
	float horz_tochecky;

	WallHorzx = data->x_intercept;
	WallHorzy = data->y_intercept;
	while (WallHorzx >= 0 && WallHorzx <= player->width * TILE_SIZE && WallHorzy >= 0 && WallHorzy <= player->height * TILE_SIZE)
	{
		horz_tochecky = WallHorzy;
		if(data->ray_up)
			horz_tochecky -= 1;
		if(isWall(WallHorzx, horz_tochecky, player))
		{
			data->Wall_x = WallHorzx;
			data->Wall_y = WallHorzy;
			data->found_wall = TRUE;
			break;
		}
		WallHorzx += data->xstep;
		WallHorzy += data->ystep;
	}
	if(data->found_wall)
		data->distance = distance(player, data->Wall_x, data->Wall_y);
	else
		data->distance = MAX_INT;
}

void vert_distance(t_ray_steps *data, t_player *player, float angle)
{
	float Wallvertx;
	float Wallverty;
	float vert_tocheckx;

	Wallvertx = data->x_intercept;
	Wallverty = data->y_intercept;
	while (Wallvertx >= 0 && Wallvertx <= player->width * TILE_SIZE && Wallverty >= 0 && Wallverty <= player->height * TILE_SIZE)
	{
		vert_tocheckx = Wallvertx;
		if(data->ray_left)
			vert_tocheckx -= 1;
		if(isWall(vert_tocheckx, Wallverty, player))
		{
			data->found_wall = TRUE;
			if(Wallvertx <= 1)
				data->Wall_x = Wallvertx - 64;
			else
				data->Wall_x = Wallvertx;
			data->Wall_y = Wallverty;
			break;
		}
		Wallvertx += data->xstep;
		Wallverty += data->ystep;
	}
	if(data->found_wall)
		data->distance = distance(player, data->Wall_x, data->Wall_y);
	else
		data->distance = MAX_INT;
}
void cast_ray(t_player *player, float angle, int rayid)
{
	t_ray_steps horz_result;
	t_ray_steps vert_result;

	angle = remainder(angle, 2 * M_PI);
	if(angle < 0)
		angle += 2 * M_PI;
	horz_result = get_horz_steps(player, angle);
	vert_result = get_vert_steps(player, angle);
	horz_distance(&horz_result, player, angle);
	vert_distance(&vert_result, player, angle);
	if(horz_result.distance < vert_result.distance)
		fill_data(player->ray, angle, horz_result, 0);
	else if(vert_result.distance < horz_result.distance)
		fill_data(player->ray, angle, vert_result, 1);
}
void get_rays(t_player *player)
{
	int rayid = 0;
	float projec = (900 / tan((FOV / 2)));
	while (rayid < 1500)
	{
		float angle2 = player->rotationAngle + atan((rayid - (1500 / 2)) / projec);
		cast_ray(player, angle2, rayid);
		rayid++;
	}
}

unsigned int get_color(int y, int x, t_data *img)
{
	char *dst;

	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *) dst);
}

void put_stripin3D(t_player *player, float project_height, int index)
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
	put_y = (int) ((900 / 2) - (project_height / 2));
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
			int Yoffset = ((Y_loop + ((project_height / 2) - (900 / 2))) * ((float) (TILE_SIZE / project_height)));
			if(Yoffset < 0)
				Yoffset = 0;
			if(Yoffset > 63 || Xoffset > 63)
				break ;
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
		while (floor_y < 900)
			my_mlx_pixel_put(&player->img, index, floor_y++, player->data->FLOOR_COLOR);
	}
	else
	{
		y = 0;
		while (y < 900)
		{
			int Yoffset = (Y_loop + ((project_height / 2) - (900 / 2))) * ((float) (TILE_SIZE / project_height));
			if(Yoffset < 0)
				Yoffset = 0;
			if(Yoffset > 63 || Xoffset > 63)
				break ;
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
	distance_toprojection = 900 / tan((FOV / 2));
	while (i < 1500)
	{
		projection_wall_height = (TILE_SIZE / (player->ray[i].distance * cos(player->ray[i].ray_angle - player->rotationAngle))) * distance_toprojection;
		put_stripin3D(player, projection_wall_height, i);
		i++;
	}
}

void init_keys(t_player *player, int key)
{
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
}
void get_value_back(t_player *player)
{
	player->walkDirection = 0;
	player->turnDirection = 0;
	player->moveDirection = 0;
}
int next_frame(int key, t_player *player)
{
	if(key == 53)
	{
		free(player->ray);
		free(player->width_for_each);
		exit(1);
	}
	init_keys(player, key);
	mlx_clear_window(player->image.mlx, player->image.win);
	change_player_status(player);
	get_rays(player);
	render_3D(player);
	mlx_put_image_to_window(player->image.mlx, player->image.win, player->img.img, 0, 0);
	get_value_back(player);
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

void init_player(t_directions *path, t_player *player)
{
	player->x = (path->PLAYER_X + 0.5) * TILE_SIZE;
	player->y = (path->PLAYER_Y + 0.5) * TILE_SIZE;
	player->height = get_height(path);
	player->width = get_width(path);
	player->width_for_each = get_widths(path, player->height);
	player->turnDirection = 0;
	player->walkDirection = 0;
	player->tab_press  = 0;
	player->rotationAngle = get_init_pos(path);
	player->walkSpeed = 0.60 * TILE_SIZE;
	player->turnSpeed = 10 * (M_PI / 180);
	player->image.mlx = mlx_init();
	player->image.win = mlx_new_window(player->image.mlx, 1500, 900, "Cub3d");
	player->data = path;
	player->ray = malloc(1500 * sizeof(t_ray));
}

void	create_images(t_player *player)
{
	player->img.img = mlx_new_image(player->image.mlx, 1500, 900);
	player->img.addr = mlx_get_data_addr(player->img.img, &player->img.bits_per_pixel, &player->img.line_length,
    	&player->img.endian);
	player->img1.img = mlx_xpm_file_to_image(player->image.mlx, "path_to_the_east_texture", &player->pic_width,&player->pic_height);
	player->img1.addr = mlx_get_data_addr(player->img1.img, &player->img1.bits_per_pixel, &player->img1.line_length,
    	&player->img1.endian);
	player->img2.img = mlx_xpm_file_to_image(player->image.mlx, "path_to_the_north_texture", &player->pic_width,&player->pic_height);
	player->img2.addr = mlx_get_data_addr(player->img2.img, &player->img2.bits_per_pixel, &player->img2.line_length,
		 &player->img2.endian);
	player->img3.img = mlx_xpm_file_to_image(player->image.mlx, "path_to_the_west_texture", &player->pic_width,&player->pic_height);
	player->img3.addr = mlx_get_data_addr(player->img3.img, &player->img3.bits_per_pixel, &player->img3.line_length,
    	&player->img3.endian);
	player->img4.img = mlx_xpm_file_to_image(player->image.mlx, "path_to_the_south_texture", &player->pic_width,&player->pic_height);
	player->img4.addr = mlx_get_data_addr(player->img4.img, &player->img4.bits_per_pixel, &player->img4.line_length,
    	&player->img4.endian);
}
void start_game(t_directions *path)
{
	t_player player;

	init_player(path, &player);
	create_images(&player);
	get_rays(&player);
	render_3D(&player);
	mlx_put_image_to_window(player.image.mlx, player.image.win, player.img.img, 0, 0);
	mlx_hook(player.image.win, 2, 0, next_frame, &player);
	mlx_hook(player.image.win, 3, 0, stop, NULL);
	mlx_hook(player.image.win, 17, (1L<<15) , close_win, &player);
	mlx_loop(player.image.mlx);
}