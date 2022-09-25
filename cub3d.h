/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:26:26 by ael-hiou          #+#    #+#             */
/*   Updated: 2022/09/09 11:59:33 by ael-hiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "includes/get_next_line.h"
#include "library/library.h"
#include <mlx.h>
#include <math.h>
#include <stdlib.h>

#define FLOOR 'F'
#define CEILLING 'C'
#define TRUE 1
#define FALSE 0
#define SPACE ' '
#define COMMA ','
#define NEW_LINE '\n'
#define ZERO '0'
#define TAB '	'
#define ONE '1'
#define TILE_SIZE 64
#define MAX_INT  2147483647
#define FOV      (60 * (M_PI / 180))
#define MINIMAP_FACTOR 0.3

typedef struct directions
{
	char	START_POSITION;
	int     PLAYER_X;
	int		PLAYER_Y;
	int		FLOOR_COLOR;
	int		CEILING_COLOR;
	char	*NORTH;
	char	*WEST;
	char	*EAST;
	char	*SOUTH;
	char	**map;
}
				t_directions;
typedef struct checking_rgb
{
	int	i;
	int start;
	int end;
	int	commas_counter;
	int	color_number;
	int	value;
	int	base;
	char	*number;	
}			t_checking_rgb;

typedef struct getting_textures
{
	int				map_size;
	int				i;
	char			**s;
	char			**data;
}					t_getting_textures;

void	checking_rgb_format_utils(char *directions, t_checking_rgb *var);
void	checking_rgb_format_init(t_checking_rgb *var);
int		checking_rgb_format(char *directions);
void	choosing_directions(char *direction, t_directions *path);
void	ft_free(char **data);
void	error_message(char *message);
void	init(t_directions *path);
void	is_surrounded_by_walls(char **map, int map_height);
void	map_validation(t_directions *path);
void	error_message(char *message);
int		get_size(char **data);
int		get_allocation_size(char *data);
void	start_game(t_directions *path);
typedef struct s_img
{
	char	*relative_path;
	int		img_width;
	int		img_height;
	void	*mlx;
	void	*img;
	void	*win;
}	t_img;

typedef struct  s_data {
	void    *img;
	char    *addr;
	int             bits_per_pixel;
	int             line_length;
	int             endian;
}	t_data;
typedef struct s_ray
{
	float distance;
	float ray_angle;
	float Wallhitx;
	float Wallhity;
	int   was_hit_vertical;
	int isRay_left;
	int isRay_right;
	int isRay_down;
	int isRay_up;
}	t_ray;

typedef struct s_player
{
	float		x;
	float		y;
	int height;
	int width;
	int turnDirection;
	int walkDirection;
	int moveDirection;
	int	tab_press;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	int pic_width;
	int pic_height;
	t_data picture;
	t_img	i;
	t_data img;
	t_directions *data;
	t_data d_dimage;
	t_ray  *ray;
	int *fixes;
}	t_player;
