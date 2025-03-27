#ifndef LED_MATRIX_HPP
# define LED_MATRIX_HPP


typedef struct s_mlx_img
{
	void			*img_ptr;
	char			*img_addr;
	int				bpp;
	int				line_len;
	int				endian;
}					t_mlx_img;

#endif