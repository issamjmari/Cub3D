#include "../cub3d.h"
#include <stdio.h>

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

	start_y = (y * 64) * MINIMAP_FACTOR;
	end_y = ((y * 64) + 64) * MINIMAP_FACTOR;
	end_x = ((x * 64) + 64) * MINIMAP_FACTOR;
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
		my_mlx_pixel_put(&player->img, (pixelx * MINIMAP_FACTOR), \
	(pixely * MINIMAP_FACTOR), color);
		pixelx += deltax;
		pixely += deltay;
		--pixels;
	}
}

int	iswall(float a, float b, t_player *player)
{
	int	wallcheckx;
	int	wallchecky;

	if (a < 0 || a > player->width * TILE_SIZE
		|| b < 0 || b > player->height * TILE_SIZE)
		return (1);
	wallcheckx = floor(a / 64);
	wallchecky = floor(b / 64);
	if (wallchecky >= player->height
		|| wallcheckx >= player->width_for_each[wallchecky]
		|| wallchecky < 0 || wallcheckx < 0)
		return (1);
	return (player->data->map[wallchecky][wallcheckx] == '1');
}

void	set_player(float a, float b, t_player *player)
{
	player->x = a;
	player->y = b;
}

void	change_player_status(t_player *player)
{
	float	newrotation;
	float	movea;
	float	moveb;
	float	a;
	float	b;

	player->rotationAngle += player->turnDirection * player->turnSpeed;
	if (player->rotationAngle != M_PI / 2)
		newrotation = player->rotationAngle - (M_PI / 2);
	a = player->x + cos(player->rotationAngle) * \
	(player->walkDirection * player->walkSpeed);
	b = player->y + sin(player->rotationAngle) * \
	(player->walkDirection * player->walkSpeed);
	if (!iswall(a, b, player))
		set_player(a, b, player);
	if (player->moveDirection != 0)
	{
		movea = player->x + (15 * player->moveDirection * cos(newrotation));
		moveb = player->y + (15 * player->moveDirection * sin(newrotation));
		if (!iswall(movea, moveb, player))
			set_player(movea, moveb, player);
	}
}

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

void	handle_multiple3d(t_threed_handle *data, t_player *player, \
int index, float project_height)
{
	int	yoffset;

	while (data->ceil_y < data->put_pos)
		my_mlx_pixel_put(&player->img, index, data->ceil_y++, \
		player->data->CEILING_COLOR);
	while (data->ceil_y < data->floor_y)
	{
		yoffset = ((data->loop++ + ((project_height / 2) - (900 / 2))) * \
	((float)(TILE_SIZE / project_height)));
		if (yoffset < 0)
			yoffset = 0;
		if (yoffset > 63 || data->Xoffset > 63)
			break ;
		put_image_strip(player, index, data, yoffset);
	}
	while (data->floor_y < 900)
		my_mlx_pixel_put(&player->img, index, data->floor_y++, \
		player->data->FLOOR_COLOR);
}

void	handle_single3d(t_threed_handle *data, t_player *player, \
int index, float project_height)
{
	int	y;
	int	yoffset;

	y = 0;
	while (y < 900)
	{
		yoffset = (data->loop++ + ((project_height / 2) - (900 / 2))) \
	* ((float)(TILE_SIZE / project_height));
		if (yoffset < 0)
			yoffset = 0;
		if (yoffset > 63 || data->Xoffset > 63)
			break ;
		if (player->ray[index].ray_content == 1)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->Xoffset, &player->img1));
		else if (player->ray[index].ray_content == 2)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->Xoffset, &player->img2));
		else if (player->ray[index].ray_content == 3)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->Xoffset, &player->img3));
		else if (player->ray[index].ray_content == 4)
			my_mlx_pixel_put(&player->img, index, y++, \
			get_color(yoffset, data->Xoffset, &player->img4));
	}
}

void	put_stripin3d(t_player *player, float project_height, int index)
{
	t_threed_handle	data;

	if (player->ray[index].was_hit_vertical)
		data.Xoffset = (int)(player->ray[index].Wallhity) % TILE_SIZE;
	else
		data.Xoffset = (int)(player->ray[index].Wallhitx) % TILE_SIZE;
	data.put_pos = (int)((900 / 2) - (project_height / 2));
	if (data.put_pos < 0)
		data.put_pos = 0;
	data.ceil_y = 0;
	data.floor_y = data.put_pos + project_height;
	data.loop = data.put_pos;
	if (data.put_pos > 0)
		handle_multiple3d(&data, player, index, project_height);
	else
		handle_single3d(&data, player, index, project_height);
}

void	render_3d(t_player *player)
{
	int		i;
	float	distance_toprojection;
	float	projection_wall_height;

	i = 0;
	distance_toprojection = 900 / tan((FOV / 2));
	while (i < 1500)
	{
		projection_wall_height = (TILE_SIZE / (player->ray[i].distance * \
	cos(player->ray[i].ray_angle - player->rotationAngle))) \
	* distance_toprojection;
		put_stripin3d(player, projection_wall_height, i);
		i++;
	}
}

void	init_keys(t_player *player, int key)
{
	if (key == 13 || key == 126)
		player->walkDirection = 1;
	if (key == 123)
		player->turnDirection = -1;
	if (key == 0)
		player->moveDirection = 1;
	if (key == 2)
		player->moveDirection = -1;
	if (key == 1 || key == 125)
		player->walkDirection = -1;
	if (key == 124)
		player->turnDirection = 1;
}

void	get_value_back(t_player *player)
{
	player->walkDirection = 0;
	player->turnDirection = 0;
	player->moveDirection = 0;
}

int	next_frame(int key, t_player *player)
{
	if (key == 53)
	{
		free(player->ray);
		free(player->width_for_each);
		exit(1);
	}
	init_keys(player, key);
	mlx_clear_window(player->image.mlx, player->image.win);
	change_player_status(player);
	get_rays(player);
	render_3d(player);
	mlx_put_image_to_window(player->image.mlx, \
		player->image.win, player->img.img, 0, 0);
	get_value_back(player);
	return (0);
}

int	stop(void)
{
	return (0);
}

float	get_init_pos(t_directions *path)
{
	if (path->START_POSITION == 'N')
		return ((3 * M_PI) / 2);
	if (path->START_POSITION == 'W')
		return (M_PI);
	if (path->START_POSITION == 'S')
		return ((M_PI / 2));
	if (path->START_POSITION == 'E')
		return (0);
	return (0);
}

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

int	close_win(t_player *player)
{
	free(player->width_for_each);
	free(player->ray);
	exit(1);
}

void	init_player(t_directions *path, t_player *player)
{
	player->x = (path->PLAYER_X + 0.5) * TILE_SIZE;
	player->y = (path->PLAYER_Y + 0.5) * TILE_SIZE;
	player->height = get_height(path);
	player->width = get_width(path);
	player->width_for_each = get_widths(path, player->height);
	player->turnDirection = 0;
	player->walkDirection = 0;
	player->tab_press = 0;
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
	player->img.addr = mlx_get_data_addr(player->img.img, \
	&player->img.bits_per_pixel, &player->img.line_length,
			&player->img.endian);
	player->img1.img = mlx_xpm_file_to_image(player->image.mlx, \
	"path_to_the_east_texture", &player->pic_width, &player->pic_height);
	player->img1.addr = mlx_get_data_addr(player->img1.img, \
	&player->img1.bits_per_pixel, &player->img1.line_length,
			&player->img1.endian);
	player->img2.img = mlx_xpm_file_to_image(player->image.mlx, \
	"path_to_the_north_texture", &player->pic_width, &player->pic_height);
	player->img2.addr = mlx_get_data_addr(player->img2.img, \
	&player->img2.bits_per_pixel, &player->img2.line_length,
			&player->img2.endian);
	player->img3.img = mlx_xpm_file_to_image(player->image.mlx, \
	"path_to_the_west_texture", &player->pic_width, &player->pic_height);
	player->img3.addr = mlx_get_data_addr(player->img3.img, \
	&player->img3.bits_per_pixel, &player->img3.line_length,
			&player->img3.endian);
	player->img4.img = mlx_xpm_file_to_image(player->image.mlx, \
	"path_to_the_south_texture", &player->pic_width, &player->pic_height);
	player->img4.addr = mlx_get_data_addr(player->img4.img, \
	&player->img4.bits_per_pixel, &player->img4.line_length,
			&player->img4.endian);
}

void	start_game(t_directions *path)
{
	t_player	player;

	init_player(path, &player);
	create_images(&player);
	get_rays(&player);
	render_3d(&player);
	mlx_put_image_to_window(player.image.mlx, player.image.win, \
	player.img.img, 0, 0);
	mlx_hook(player.image.win, 2, 0, next_frame, &player);
	mlx_hook(player.image.win, 3, 0, stop, NULL);
	mlx_hook(player.image.win, 17, (1L << 15), close_win, &player);
	mlx_loop(player.image.mlx);
}
