#include "../include/Mlx.hpp"

Mlx::Mlx(int width, int height) : _width(width), _height(height)
{
    this->mlx_ptr = mlx_init();
    if (!this->mlx_ptr)
        throw (MlxError());
    this->win_ptr = mlx_new_window(this->mlx_ptr, width, height, (char *)"LED_MATRIX");
    if (!this->win_ptr)
    {
        mlx_destroy_display(this->mlx_ptr);
        free(mlx_ptr);
        throw (MlxError());
    }
}

int Mlx::getWidth(void)
{
    return this->_width;
}

int Mlx::getHeight(void)
{
    return this->_height;
}

Mlx::Mlx() {}

Mlx::~Mlx()
{
    // destroy mlx clean
}