#ifndef INCLUDE_H
#define INCLUDE_H

#define W_WIDTH 800
#define W_HEIGHT 600
#define G_WIDTH 20
#define G_HEIGHT 15

# define GRID_X 40
# define GRID_Y 40

# define ALIVE_COLOR 0x00FF00
# define DEAD_COLOR 0x000000
# define GRID_COLOR 0x0000FF

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define UP_KEY      65362
#define DOWN_KEY    65364

typedef struct s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
	int				width;
	int				height;
}					t_mlx;

typedef struct s_img
{
	void			*img_ptr;
	char			*img_addr;
	int				bpp;
	int				line_len;
	int				endian;
}					t_img;

typedef struct cell
{
    int x;
    int y;
}                   t_cell;

/******* MLX ********/
t_mlx	start_mlx(int width, int height);
t_img	*init_img(t_mlx *window);
int	    *get_pixel_from_img(t_img *img, int x, int y);
void	img_pix_put(t_img *img, int x, int y, int color);
void    draw_cell(t_img *img, int x, int y, int color);
void    draw_grid(t_img *img, int color);
void    push_img(t_img *img, t_mlx *window);

int    radial_loop(t_mlx *window);
void    linear_loop(t_mlx *window);
int     conway_loop(t_mlx *param_window);

int     wheel(__uint8_t pos);

/*********** COLORS ********** */
int	create_trgb(int t, int r, int g, int b);
void	palette_one(int *colors);
void	palette_two(int *colors);
void	palette_three(int *colors);
void	palette_four(int *colors);
int	    interpolate_color(int color_a, int color_b, float t);
int	    get_color_gradient(float iter, int *colors, int nb_colors);


#endif