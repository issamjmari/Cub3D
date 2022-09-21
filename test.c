#include <mlx.h>
#include <stdio.h>
int main()
{
	int width;
	int height;
	int bits_per_pixel;
	int size_line;
	int endian;
	void *mlx = mlx_init();
	void *win = mlx_new_window(mlx, 750, 400, "Cub3d");
	void *photo = mlx_xpm_file_to_image(mlx, "a.xpm", &width, &height);
	char *f = mlx_get_data_addr(photo, &bits_per_pixel, &size_line, &endian);
	char	*dst;
	printf("%d and %d\n", size_line, bits_per_pixel);
	dst = f + (324 * size_line + 499 * (bits_per_pixel / 8));
	// *(unsigned int *) dst = 55;
	printf("%u\n", *(unsigned int *) (dst));
}