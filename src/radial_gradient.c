#include "../include/include.h"

float branches = -2.0f;
float pixellization = 10; // 2.90f; // 1,
float spiral_speed = 0.01f;
float anim_speed = 0.002f;
int   color_mode = 0;
int   nb_colors = 0;
int   next_color = 2;
int   mode = 0;
int   active_palette[5];

int   transition_start_frame = 0;
int   restart_frame = 0;
int   animation_restart = 0;

float ring_radius;
int   ring_active;
int   ring_start_frame;

int   leds[15][20];
int   frame = 0;
int   cell_state[G_HEIGHT][G_WIDTH] = {0};

int handle_keys(int key)
{
    if (key == 65307)
        exit(0);
    else if (key == UP_KEY && mode == 0)
        branches += 0.1f;
    else if (key == DOWN_KEY && mode == 0)
        branches -= 0.1f;
    else if (key == 119 && mode == 0)
        pixellization += 0.1f;
    else if (key == 115 && mode == 0)
        pixellization -= 0.1f;
    else if (key == UP_KEY && mode == 2)
        spiral_speed += 0.01f;
    else if (key == DOWN_KEY && mode == 2)
        spiral_speed -= 0.01f;
    else if (key == 65361)
        mode = (mode + 1 ) % 3;
    else if (key == 100)
        color_mode = (color_mode + 1) % 5;
    else if (key == 110)
    {
        // transition_start_frame = frame;
        ring_active = 1;
        ring_start_frame = frame;
    }
    else if (key == 107)
        nb_colors = (nb_colors + 1) % 2;
    else if (key == 61)
        anim_speed += 0.0001;
    else if (key == 45 && anim_speed > 0)
        anim_speed -= 0.0001;
    // printf("b = %f p = %f\n", branches, pixellization);
    // printf("anim speed = %f\n", anim_speed);
    // printf("color mode = %d\n", color_mode);
    printf("key = %d\n",key);
    // printf("form = %f\n", *form);
    return 0;
}

void reset_cell_state()
{
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
            cell_state[i][j] = 0;
    }
}

float calculate_radient(t_cell *center, float max_distance, int i, int j)
{
    t_cell cell = {i, j};

    cell.x = i;
    cell.y = j;
            
    float angle = atan2(cell.y - center->y, cell.x - center->x);
    float distance = get_norm_distance(&cell, center, max_distance);

    float form = -8.0f + 2.0f * sin(frame * 0.02f); // pulsation douce

    // float pulse = sin(distance * 0.1f + frame * 0.01f);
    // float r = distance * (form + (pixellization * pulse * sin(branches * angle)));

    float r = distance * (form + (pixellization * sin(branches * angle)));

    float raw_color = (r / max_distance + (float)frame * anim_speed);
    return (fmodf(fabsf(raw_color), 1.0f));
}

void radial_gradient()
{
    t_cell cell;
    t_cell center = {9, 7};
    float max_distance = 12.5;
    int   colors[5];
    float color;

    // static transition_radius = 0;
    
    // int colors_nb = nb_colors;

    switch (color_mode)
    {
        case 0:
            palette_one(colors);
            break;

        case 1:
            palette_two(colors);
            break;

        case 2:
            palette_three(colors);
            break;

        case 3:
            palette_four(colors);
            break;

        default:
            break;
    }
    // float form = -10.0f;         // -2, -200

    float anim_relaunch_radius = clamp((frame - restart_frame) * 0.005f, 0.0f, 1.0f);

    float ring_radius = 0.0f; // avance de 0 à max_distance
    float ring_thickness = 0.1f; // plus petit = plus fin
    static int ring_color_idx = 0;
    int ring_color = colors[ring_color_idx];

    if (ring_active)
    {
        float ring_speed = 0.02f;  // plus petit = plus lent
        // ring_radius = (frame - ring_start_frame) * ring_speed;
        float t = clamp((frame - ring_start_frame) * ring_speed / max_distance, 0.0f, 1.0f);
        ring_radius = max_distance * (t < 0.5f
        ? 2.0f * t * t          // ease-in
        : -1.0f + (4.0f - 2.0f * t) * t); // ease-out


        printf("ring radius = %f\n", ring_radius);
        if (ring_radius > 1) {
            ring_active = 0; // fin de l'effet
            printf("END\n");
            ring_color_idx++;
            if (ring_color_idx == 5)
                ring_color_idx = 0;
        }
        
    }
    
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            float distance = get_norm_distance(&cell, &center, max_distance);
            // float delta = fabsf(distance - ring_radius); // distance au cercle animé
            color = calculate_radient(&center, max_distance, i, j);
            

            if (transition_start_frame)
            {
                float propagation = clamp((frame - transition_start_frame) * 0.005f, 0.0f, 1.0f);
                
                if (distance <= propagation)
                    cell_state[i][j] = 1;

                if (cell_state[i][j] == 1)
                    leds[j][i] = colors[next_color];
                else
                    leds[j][i] = get_color_gradient(color, colors, nb_colors);
            }
            else if (animation_restart)
            {
                // printf("anim restart\n");
                if (distance > anim_relaunch_radius)
                    leds[j][i] = colors[next_color - 1];
                else
                    leds[j][i] = get_color_gradient(color, colors, nb_colors);
                    // Quand relance anim est finie, on désactive le mode "reprise"
            if (animation_restart && anim_relaunch_radius >= 1.0f)
            {
                printf("stop anim\n");
                animation_restart = 0;
            }
            }
            if (ring_active && fabsf(distance - ring_radius) < ring_thickness)
                leds[j][i] = ring_color;
            else
                leds[j][i] = get_color_gradient(color, colors, 5);
            // draw_cell(img, i, j, get_color_gradient(color, colors, 5));
                        // printf("i = %d j = %d\n", i, j);
        }
    }
    // push_img(img, window);
    if (transition_start_frame)
    {
        int all_cells_converted = 1;
        for (int i = 0; i < G_WIDTH; i++)
        {
            for (int j = 0; j < G_HEIGHT; j++)
            {
                if (cell_state[i][j] == 0)
                {
                    all_cells_converted = 0;
                    break;
                }
            }
            if (!all_cells_converted)
                break;
        }
    
        if (all_cells_converted)
        {
            printf("All cells converted\n");
            reset_cell_state(); 
            nb_colors = (nb_colors + 1) % 5;     // ajoute la nouvelle couleur
            next_color = (next_color + 1) % 5;
            printf("nb_colors = %d\n", nb_colors);
            transition_start_frame = 0;
            restart_frame = frame;
            animation_restart = 1;
        }
    }
}

long int	get_time_elapsed(t_timeval *starting_time)
{
	t_timeval	current_time;
	long int	time_elapsed;

	gettimeofday(&current_time, NULL);
	time_elapsed = ((current_time.tv_sec - starting_time->tv_sec) * 1000)
		+ ((current_time.tv_usec - starting_time->tv_usec) * 0.001);
	return (time_elapsed);
}

//     // for (int i = 0; i < 5; i++)
//     //     active_palette[i] = interpolate_color(colors_1[i], colors_2[i], distance);

int radial_loop(void *data)
{
    float               target_frame_time_ms = 33.333f; // 1000 / 60 (fps)
    static float        distance = -1.0f;
    long int            frame_time;
    int                 colors_1[5];
    int                 colors_2[5];
    t_timeval           timer;
    sensor_data_t       *sensor_data;

    sensor_data = (sensor_data_t *)data;
    palette_one(colors_1);
    palette_two(colors_2);

    get_sensor_values(sensor_data, &distance);
    
    // Lissage EMA
    float alpha = 0.05f;  // rapide mais fluide
    
    pthread_mutex_lock(sensor_data->avg_lock);
    sensor_data->interp += alpha;
    if (sensor_data->interp > 1.0f)
        sensor_data->interp = 1.0f;
    distance = lerp(sensor_data->last_value, sensor_data->next_value, sensor_data->interp);
    pthread_mutex_unlock(sensor_data->avg_lock);

    float curved = powf(normalize_value(distance, 0.0f, 50.0f), 1.2f); // légère courbe pour douceur
    pixellization = curved * 12.0f;
        
    // printf("di = %f interp = %f last = %f next = %f pix = %f\n", distance, sensor_data->interp, sensor_data->last_value, sensor_data->next_value, pixellization);

    radial_gradient();
    frame_time = get_time_elapsed(&timer);
    if (target_frame_time_ms < frame_time)
        usleep((target_frame_time_ms - frame_time) * 1000);
    t_img *img = init_img(sensor_data->window);
    for (int i = 0; i < G_HEIGHT; i++)
    {
        for (int j = 0; j < G_WIDTH; j++)
            draw_cell(img, j, i, leds[i][j]);
    }
    push_img(img, sensor_data->window);
    frame++;
    return (0); 
}

void distance_thread_routine(void *data)
{
    sensor_data_t *sensor_data = (sensor_data_t *)data;

    while (1)
    {
        update_average_distance(sensor_data);
        usleep(1); // toutes les 50 ms
    }
}

int start_radial(t_mlx *window)
{
    pthread_t       sensor_thread;
    pthread_mutex_t avg_mutex;
    pthread_mutex_t interp_mutex;
    sensor_data_t   sensor_data;

    if (pthread_mutex_init(&avg_mutex, NULL) == -1 || pthread_mutex_init(&interp_mutex, NULL) == -1)
        return (-1);
    sensor_data.average = 0;
    sensor_data.last_value = 0;
    sensor_data.next_value = 0;
    sensor_data.interp = 0;
    sensor_data.avg_lock = &avg_mutex;
    sensor_data.interp_lock = &interp_mutex;
    sensor_data.window = window;
    sensor_data.uart_fd = open(SERIAL_PORT, O_RDONLY | O_NOCTTY);
    if (sensor_data.uart_fd == -1) 
    {
        perror("Erreur d'ouverture du port série");
        return 1;
    }
    // Configurer le port série
    if (configure_serial_port(sensor_data.uart_fd) < 0) 
    {
        close(sensor_data.uart_fd);
        return 1;
    }
    void *data = &sensor_data;
    if (pthread_create(&sensor_thread, NULL, (void *)distance_thread_routine, data) == -1)
        return (-1);
    mlx_hook(window->win_ptr, 2, 1L << 0, handle_keys, &pixellization);
    mlx_loop_hook(window->mlx_ptr, &radial_loop, data);
    mlx_loop(window->mlx_ptr);
    return (0);
}