#include <mlx.h>
#include <math.h>
typedef struct  s_data {
	void    *img;
	char    *addr;
	int             bits_per_pixel;
	int             line_length;
	int             endian;
}	t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
void draw_line(t_data *data, float begX, float begY, float endX, float endY, int color)
{
	double deltaX = (endX - begX);
	double deltaY = (endY - begY);
	int pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	deltaX /= pixels;
	deltaY /= pixels;
	double pixelX = begX;
	double pixelY = begY;
	while (pixels)
	{
	    my_mlx_pixel_put(data, ((pixelX)), ((pixelY)), color);
	    pixelX += deltaX;
	    pixelY += deltaY;
	    --pixels;
	}
}
int main()
{
	t_data data;
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, 1800, 1200, "Cub3d");

	data.img = mlx_new_image(mlx, 180, 180);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);
	int x = 180 / 2;
	int y = 0;
	int rad = 15;
	while (y < 180)
	{
		draw_line(&data, x - rad, y, x + rad, y, 16000547);
		if(y < 90)
			rad += 4;
		else
			rad -= 4;
		y++;
	}
	mlx_put_image_to_window(mlx, win, data.img, 15, 15);
	mlx_loop(mlx);
}