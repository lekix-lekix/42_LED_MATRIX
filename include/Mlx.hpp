#include <iostream>
#include <exception>
#include <cstdlib>
# include "../minilibx-linux/mlx.h"

class Mlx
{
    private:
        int             _width;
        int             _height;
        void			*mlx_ptr;
        void			*win_ptr;

    public:
        Mlx(int height, int width);
        int getWidth();
        int getHeight();
    
        Mlx();
        ~Mlx();
};

class MlxError : public std::exception {
	public:
		const char *what(void) const throw();
};
