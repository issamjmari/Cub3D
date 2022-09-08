#ifndef CUB3D
#define CUB3D
#include <mlx.h>
#include <math.h>
#define FALSE 0
#define TRUE 1
#define TILE_SIZE 50
#define FOV      (60 * (M_PI / 180))

typedef struct s_img
{
	char	*relative_path;
	int		img_width;
	int		img_height;
	void	*mlx;
	void	*img;
	void	*win;
}	t_img;

typedef struct s_ray
{
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
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
	t_img	i;
}	t_player;

#endif