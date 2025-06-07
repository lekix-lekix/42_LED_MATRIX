#include "../include/include.h"

int main ()
{
    t_mlx window = start_mlx(W_WIDTH, W_HEIGHT);

    // conway_loop(&window);
    // linear_loop(&window);
    start_radial(&window);
}
