#include "../include/include.h"

int wheel(__uint8_t pos) 
{
    __uint8_t r;
    __uint8_t g;
    __uint8_t b;

    pos = 255 - pos;
    if (pos < 85)
    {
        r = 255 - pos * 3;
        g = 0;
        b = pos * 3;
    }
    else if (pos < 170) 
    {
        pos = pos - 85;
        r = 0;
        g = pos * 3;
        b = 255 - pos * 3;
    }
    else
    {
        pos = pos - 170;
        r = pos * 3;
        g = 255 - pos * 3;
        b = 0;
    }
    return (r << 16 | g << 8 | b);
}

int main ()
{
    t_mlx window = start_mlx(W_WIDTH, W_HEIGHT);

    // conway_loop(&window);
    // linear_loop(&window);
    radial_loop(&window);
}
