/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijmari <ijmari@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:26:26 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/10/12 14:39:02 by ijmari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "get_next_line.h"
# include "library.h"
# include <mlx.h>
# include <math.h>
# include <stdlib.h>

# define DOUBLE_NEW_LINE "\033[0;31mError \nMAP Contain Multiple New Lines"
# define CEILING_COLOR_MISSING "\033[0;31mError \nCeiling Color Is Missing"
# define FLOOR_COLOR_MISSING "\033[0;31mError \nFloor Color Is Missing"
# define SOUTH_TEXTURE_MISSING "\033[0;31mError \nSouth Texture Is Missing"
# define NORTH_TEXTURE_MISSING "\033[0;31mError \nNorth Texture Is Missing"
# define WEST_TEXTURE_MISSING "\033[0;31mError \nWest Texture Is Missing"
# define EAST_TEXTURE_MISSING "\033[0;31mError \nEast Texture Is Missing"
# define NORTH_TEXTURE_DUPLICATE "\033[0;31mError \nNorth Texture Already Defined"
# define WEST_TEXTURE_DUPLICATE "\033[0;31mError \nWest Texture Already Defined"
# define EAST_TEXTURE_DUPLICATE "\033[0;31mError \nEast Texture Already Defined"
# define SOUTH_TEXTURE_DUPLICATE "\033[0;31mError \nSouth Texture Already Defined"
# define FLOOR_DUPLICATE "\033[0;31mError \nFloor Color Already Defined"
# define CEILING_DUPLICATE "\033[0;31mError \nCeiling Color Already Defined"
# define MISSING_PLAYER_MSG "\033[0;31mError \nMap Must Contain A Player"
# define INVALID_MAP "\033[0;31mError \nInvalid Map"
# define SURROUNDED_SPACE_MSG "\033[0;31mError \nSpace \
Should Be Surrounded By Walls"
# define SURROUNDED_MSG "\033[0;31mError \nMap Should Be Surrounded By Walls"
# define INVALID_DIRECTION_MSG "\033[0;31mError \nInvalid Input"
# define NOTFOUND_TEXTURE_MSG "\033[0;31mError \nTexture Not Found"
# define WRONG_RGB_MSG "\033[0;31mError \nWrong RGB Format"
# define PATH_ERROR "\033[0;31mError \nMap Not Found"
# define FLOOR_CEILING_MISSING_MSG "\033[0;31mError \nFloor \
Or Ceiling Color Missing"
# define UNWANTED_CHARACTER_MSG "\033[0;31mError \nUnwanted \
Character Exist"
# define CONTAIN_MORE_MSG "\033[0;31mError \nMap Mustn't \
Contain More Than 1 Player"
# define ERROR_OCCURRED "\033[0;31mError \nError occurred"
# define N "NO"
# define W "WE"
# define E "EA"
# define S "SO"
# define FLOOR 'F'
# define CEILING 'C'
# define TRUE 1
# define FALSE 0
# define SPACE ' '
# define COMMA ','
# define NEW_LINE '\n'
# define ZERO '0'
# define TAB '	'
# define ONE '1'

typedef struct map_content_vars
{
	char	*entereddata;
	char	*trimmeddata;
	int		linescounter;
	int		i;
}	t_mapContentVars;

typedef struct checkDuplicate
{
	int	fcounter;
	int	ccounter;
	int	nocounter;
	int	socounter;
	int	eacounter;
	int	wecounter;
}	t_checkDuplicate;
typedef struct directions
{
	char	startposition;
	int		player_x;
	int		player_y;
	int		floor_color;
	int		ceiling_color;
	char	*north;
	char	*west;
	char	*east;
	char	*south;
	char	**map;
}	t_directions;

typedef struct checking_rgb
{
	int		digitscounter;
	int		i;
	int		start;
	int		end;
	int		commascounter;
	int		colornumber;
	int		value;
	int		base;
	char	*number;	
}			t_checking_rgb;

typedef struct getting_textures
{
	int		map_size;
	int		i;
	char	**s;
	char	**data;
}	t_getting_textures;

typedef struct s_img
{
	char	*relative_path;
	int		img_width;
	int		img_height;
	void	*mlx;
	void	*img;
	void	*win;
}	t_img;

typedef struct s_data {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}	t_data;

typedef struct s_ray
{
	float	distance;
	float	ray_angle;
	float	wallhitx;
	float	wallhity;
	int		was_hit_vertical;
	int		ray_content;
	int		isray_left;
	int		isray_right;
	int		isray_down;
	int		isray_up;
}	t_ray;

typedef struct s_player
{
	float			x;
	float			y;
	int				*width_for_each;
	int				height;
	int				width;
	int				turndirection;
	int				walkdirection;
	int				movedirection;
	int				mouse;
	int				tab_press;
	float			rotationangle;
	float			walkspeed;
	float			turnspeed;
	int				pic_width;
	int				pic_height;
	t_data			img1;
	t_data			img2;
	t_data			img3;
	t_data			img4;
	t_data			img5;
	t_img			image;
	t_data			img;
	t_directions	*data;
	t_ray			*ray;
}	t_player;

typedef struct s_ray_steps
{
	float	x_intercept;
	float	y_intercept;
	float	xstep;
	float	ystep;
	int		ray_down;
	int		ray_up;
	int		ray_left;
	int		ray_right;
	float	distance;
	float	found_wall;
	float	wall_x;
	float	wall_y;
	int		vertical;
}	t_ray_steps;

typedef struct s_second_part_init
{
	char	*map;
	char	*entereddata;
	int		isplayerexist;
	int		i;
	int		counter;
}	t_secondPartVars;

typedef struct s_threed_handle
{
	int	xoffset;
	int	ceil_y;
	int	floor_y;
	int	loop;
	int	put_pos;
}	t_threed_handle;

void			map_first_part(char *entereddata, t_directions *path, \
t_checkDuplicate *checkDuplicate);
void			getting_map_content(int fd, t_directions *path);
void			map_first_part(char *entereddata, t_directions *path, \
t_checkDuplicate *checkDuplicate);
int				is_full_spaces(char *entereddata);
int				check_for_double_newlines(char *map);
void			missing_texture(t_checkDuplicate *vars);
void			map_content_init(t_mapContentVars *vars);
void			second_part_init(t_secondPartVars *vars);
void			unwanted_characters(t_secondPartVars *vars);
void			checking_duplicate_init(t_checkDuplicate *checkDuplicate);
void			rgb_init(t_checking_rgb *var);
void			unwantedCharactersUtils(int *isExist, char **map, \
t_directions *path, int i);
void			unwantedCharacters(char **map, t_directions *path);
void			counter_checker(int *texturecounter, char *message);
void			checking_rgb_formatUtils(char *directions, t_checking_rgb *var);
int				checking_rgb_format(char *secondPart);
void			choosing_directions(char *firstPart, char *secondPart, \
t_directions *path, t_checkDuplicate *checkDuplicate);
void			ft_free(char **data);
void			error_message(char *message);
void			path_init(t_directions *path);
void			is_surrounded_by_walls(char **map, int map_height);
void			map_validation(t_directions *path);
void			error_message(char *message);
int				get_size(char **data);
void			start_game(t_directions *path);
void			initKeys(t_player *player, int key);
float			getInitPos(t_directions *path);
void			init_player(t_directions *path, t_player *player);
void			get_value_back(t_player *player);
void			set_player(float a, float b, t_player *player);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);
void			ft_draw_elem(int x, int y, t_player *player, int color);
void			draw_line(t_player *player, float endX, float endY, int color);
unsigned int	get_color(int y, int x, t_data *img);
void			put_image_strip(t_player *player, int index, \
t_threed_handle *data, int yoffset);
void			handle_multiple3d(t_threed_handle *data, t_player *player, \
int index, float project_height);
void			handle_single3d(t_threed_handle *data, t_player *player, \
int index, float project_height);
void			put_stripin3d(t_player *player, \
float project_height, int index);
void			render_3d(t_player *player);
int				get_width(t_directions *path);
int				get_height(t_directions *path);
int				*get_widths(t_directions *path, int height);
void			create_images(t_player *player);
int				close_win(t_player *player);
int				stop(void);
int				next_frame(int key, t_player *player);
float			distance(t_player *p, float Wallx, float Wally);
void			fill_data(t_ray *ray, float angle, \
t_ray_steps data, int was_vertical);
void			get_vert_steps(t_player *player, \
float angle, t_ray_steps *horz);
void			get_horz_steps(t_player *player, \
float angle, t_ray_steps *vert);
void			set_distance(t_ray_steps *data, t_player *player);
void			horz_distance(t_ray_steps *data, t_player *player);
void			vert_distance(t_ray_steps *data, t_player *player);
void			cast_ray(t_player *player, float angle);
void			get_rays(t_player *player);
int				iswall(float a, float b, t_player *player);
void			change_player_status(t_player *player, int key);
void			init_keys(t_player *player, int key);
void			check_left_angle(t_player *player);
void			check_right_angle(t_player *player);
void			check_right_move(t_player *player);
void			check_left_move(t_player *player);
void			change_player_status(t_player *player, int key);
int				check_line(t_player *player, float endX, float endY);
void			check_up(t_player *player);
void			check_down(t_player *player);
#endif 