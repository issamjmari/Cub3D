#ifndef CUB3D
#define CUB3D
#include <mlx.h>
#include <math.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1
#define TILE_SIZE 64
#define MAX_INT  2147483647
#define FOV      (60 * (M_PI / 180))
#define MINIMAP_FACTOR 0.3

float fixed_mul(double a, double b);
float fixed_oth(float a, float b);
float fixed_minus(float a, float b);
float fixed_add(float a, float b);
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
	float height;
	float width;
	int turnDirection;
	int walkDirection;
	int moveDirection;
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	int pic_width;
	int pic_height;
	t_data picture;
	t_img	i;
	t_data img;
	t_ray  *ray;
}	t_player;


#endif