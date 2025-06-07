#include "../include/include.h"

int handle_keys(int key);

float square(float nb)
{
    return (nb * nb);
}

float get_norm_distance(t_cell *cell, t_cell *center, float max_distance)
{
    float distance_squared = square(cell->x - center->x) + square(cell->y - center->y);
    return (distance_squared / square(max_distance));
}

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

int   leds[15][20];
int   frame = 0;
int   cell_state[G_HEIGHT][G_WIDTH] = {0};

float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void reset_cell_state()
{
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
            cell_state[i][j] = 0;
    }
}

void print_matrix(int matrix[15][20])
{
    for (int i = 0; i < G_HEIGHT; i++)
    {
        for (int j = 0; j < G_WIDTH; j++)
        {
            printf("%d | ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void radial_gradient(t_mlx *window)
{
    // t_img *img = init_img(window);
    (void) window;
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
    float form = -10.0f;         // -2, -200

    float anim_relaunch_radius = clamp((frame - restart_frame) * 0.005f, 0.0f, 1.0f);

    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            float angle = atan2(cell.y - center.y, cell.x - center.x);
            float distance = get_norm_distance(&cell, &center, max_distance);
            // float pulse = sin(distance * 0.1f + frame * 0.01f);
            // float r = distance * (form + (pixellization * pulse * sin(branches * angle)));
            float r = distance * (form + (pixellization * sin(branches * angle)));
            float raw_color = (r / max_distance + (float)frame * anim_speed);
            color = fmodf(fabsf(raw_color), 1.0f);

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

void radial_gradient_round(t_mlx *window, int frame)
{
    t_img *img = init_img(window);
    
    t_cell cell;
    t_cell center = {9, 7};
    float norm_distance;
    float max_distance = 12.5;
    float wave;
    
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            norm_distance = get_norm_distance(&cell, &center, max_distance);
            wave = (sin(norm_distance / 1.0f) - (frame / 500.0f) + 1) / 2;
            draw_cell(img, i, j, wheel(wave * 255));
        }
    }
    push_img(img, window);
}

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
        transition_start_frame = frame;
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

// Configuration du port série
int configure_serial_port(int fd) {
    struct termios options;

    // Récupérer les paramètres actuels du port série
    if (tcgetattr(fd, &options) < 0) {
        perror("Erreur lors de la récupération des paramètres du port série");
        return -1;
    }

    // Configurer les paramètres : 115200 bauds, 8 bits de données, pas de parité, 1 bit d'arrêt
    cfsetispeed(&options, B115200);  // vitesse de réception
    cfsetospeed(&options, B115200);  // vitesse d'émission

    options.c_cflag &= ~PARENB;    // Pas de parité
    options.c_cflag &= ~CSTOPB;    // 1 bit d'arrêt
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;        // 8 bits de données
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // mode raw
    options.c_iflag &= ~(IXON | IXOFF | IXANY);         // désactiver le flow control logiciel
    options.c_oflag &= ~OPOST;                          // mode raw output
    options.c_cc[VMIN]  = 8;                            // attendre 8 octets minimum
    options.c_cc[VTIME] = 10;         

    options.c_cflag |= CREAD | CLOCAL;  // Activer la lecture et ignorer les lignes de contrôle

    // Appliquer les paramètres configurés
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("Erreur lors de l'application des paramètres du port série");
        return -1;
    }
    return 0;
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

float read_sensor_data(int uart_fd)
{
    size_t  bytes_read;
    char    buffer[BUFFER_SIZE];
    float   distance; 

    bytes_read = read(uart_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) 
    {
        buffer[bytes_read] = '\0';
        distance = atof(buffer);
    }
    printf("buffer = %s bytes read = %ld\n", buffer, bytes_read);
    return (distance);
}

void copy_float_tab(float *src, float *dst, int size)
{
    for (int i = 0; i < size; i++)
        dst[i] = src[i];
}

void get_distance_tab(int uart_fd, float *tab, int size)
{
    float sensor_data = 0;
    if (tab[0] == -1)
    {
        for (int i = 0; i < size; i++)
        {
            sensor_data = read_sensor_data(uart_fd);
            if (sensor_data == -1)
                sensor_data = 0;
            tab[i] = sensor_data;
        }
        return ;
    }
    sensor_data = read_sensor_data(uart_fd);
    // printf("sensor data = %f\n", sensor_data);
    if (sensor_data == -1)
        sensor_data = 0;
    // printf("sensor data = %f\n", sensor_data);
    for (int i = 0; i < size - 1; i++)
        tab[i] = tab[i + 1];
    tab[size - 1] = sensor_data;
    for (int i = 0; i < size - 1; i++)
        printf("tab[%d] = %f\n", i, tab[i]);
    printf("=========\n");
}

// float get_avg_ftab(float *tab, int size)
// {
//     float avg = 0;

//     for (int i = 0; i < size; i++)
//         avg += tab[i];
//     return (avg / size);
// }

float lerp(float a, float b, float t)
{
    return (a * (1 - t) + b * t);
}

float norm_value(float value, float min, float max)
{
    if (max - min == 0.0f)
        return 0.0f;  // pour éviter une division par zéro
    float t = (value - min) / (max - min);
    if (t < 0.0f)
        return 0.0f;
    if (t > 1.0f)
        return 1.0f;
    return t;
}

// int start_radial(void *data)
// {
//     // float            target_frame_time_ms = 33.333f; // 1000 / 60 (fps)
//     // static float     distance_tab[10];
//     float               smoothed_distance = -1.0f;
//     float               distance;
//     // long int         frame_time;
//     // t_timeval        timer;
//     int colors_1[5];
//     int colors_2[5];
    
//     // __uint8_t led_data[300 * 3];

//     sensor_data_t *sensor_data = (sensor_data_t *)data;
//     palette_one(colors_1);
//     palette_two(colors_2);
//     // if (frame == 0)
//     //     get_distance_tab(window->uart_fd, distance_tab, 10, 1);
//     // else
//     //     get_distance_tab(window->uart_fd, distance_tab, 10, 0);
//     // distance = get_avg_ftab(distance_tab, 10);
//     // distance = clamp(distance, 0.0f, 100.0f);
//     // distance = norm_value(distance, 0, 50.0f);
//     // for (int i = 0; i < 10; i++)
//     //     printf("dist tab = %f\n", distance_tab[i]);
//     // printf("distance = %f\n", distance);
//     // for (int i = 0; i < 5; i++)
//     //     active_palette[i] = interpolate_color(colors_1[i], colors_2[i], distance);
//     pthread_mutex_lock(sensor_data->avg_lock);
//     distance = *(sensor_data->average);
//     pthread_mutex_unlock(sensor_data->avg_lock);
//     printf("distance in main = %f\n", distance);

//     if (smoothed_distance < 0.0f)
//         smoothed_distance = distance;

//     // Clamp contre les grosses variations d'un coup
//     float max_jump = 3.0f;
//     float delta = distance - smoothed_distance;
//     if (fabsf(delta) > max_jump)
//         distance = smoothed_distance + max_jump * ((delta > 0) ? 1 : -1);

//     // Lissage EMA (ici alpha = 0.05 pour douceur)
//     float alpha = 0.15f;
//     smoothed_distance = lerp(smoothed_distance, distance, alpha);

//     // Mapping optionnel
//     float normalized = normalize_value(smoothed_distance, 0.0f, 50.0f);
//     float curved = powf(normalized, 1.5f);  // ou sans si tu veux linéaire

//     // Application à ton visuel
//     pixellization = curved * 12.0f; // admettons que 12 est le max utile pour ton rendu

//     // smoothed_distance = lerp(smoothed_distance, distance, 0.f);
//     // pixellization = smoothed_distance;
//     // pixellization = lerp(pixellization, distance, 0.8f);
//     // printf("pixellization = %f\n", pixellization);
//     // gettimeofday(&timer, NULL);
//     radial_gradient(sensor_data->window);
//     // frame_time = get_time_elapsed(&timer);
//     // if (target_frame_time_ms < frame_time)
//     //     usleep((target_frame_time_ms - frame_time) * 1000);
//     t_img *img = init_img(sensor_data->window);
//     for (int i = 0; i < G_HEIGHT; i++)
//     {
//         for (int j = 0; j < G_WIDTH; j++)
//         {
//             // if (i % 2 != 0)
//                 // draw_cell(img, j, i, leds[i][19 - j]);  // c'est pas degueu d'imprimer cmme ca en mode reel
//             // else
//                 draw_cell(img, j, i, leds[i][j]);
//         }
//         // draw_cell(img, j, i, leds[i][j]);
//     }
//     // print_matrix(leds);
//     // int led_i;
//     // for (int i = 0; i < G_HEIGHT; i++)
//     // {
//     //     for (int j = 0; j < G_WIDTH; j++)
//     //     {
//     //         if (i % 2 != 0)
//     //         {
//     //             led_data[led_i * 3 + 0] = get_r(leds[i][19 - j]);
//     //             led_data[led_i * 3 + 1] = get_g(leds[i][19 - j]);
//     //             led_data[led_i * 3 + 1] = get_b(leds[i][19 - j]);
//     //         }
//     //         else
//     //         {
//     //             led_data[led_i * 3 + 0] = get_r(leds[i][j]);
//     //             led_data[led_i * 3 + 1] = get_g(leds[i][j]);
//     //             led_data[led_i * 3 + 1] = get_b(leds[i][j]);
//     //         }
//     //     }
//     //     draw_cell(img, j, i, leds[i][j]);
//     // }
//     push_img(img, sensor_data->window);
//     frame++;
//     return (0); 
// }

void get_sensor_values(sensor_data_t *data, float *curr_distance)
{
    pthread_mutex_lock(data->avg_lock);
    float avg = clamp(data->average, 0.0f, 100.0f);
    if (*curr_distance == -1)
    {
        *curr_distance = avg;
        pthread_mutex_unlock(data->avg_lock);
        return ;
    }
    // if (fabsf(data->average - *curr_distance) < 0.1f)
    // {
    //     pthread_mutex_unlock(data->avg_lock);
    //     return ;
    // }
    if (avg == *curr_distance)
    {
        pthread_mutex_unlock(data->avg_lock);
        return ;
    }
    data->last_value = *curr_distance;
    data->next_value = avg;
    data->interp = 0.0f;
    *curr_distance = avg;
    pthread_mutex_unlock(data->avg_lock);
}

int start_radial(void *data)
{
    float               target_frame_time_ms = 33.333f; // 1000 / 60 (fps)
    // static float     distance_tab[10];
    float               smoothed_distance = -1.0f;
    static float        distance = -1.0f;
    long int            frame_time;
    t_timeval           timer;
    int colors_1[5];
    int colors_2[5];
    
    // __uint8_t led_data[300 * 3];

    sensor_data_t *sensor_data = (sensor_data_t *)data;
    palette_one(colors_1);
    palette_two(colors_2);

    if (smoothed_distance < 0.0f)
        smoothed_distance = distance;
    
    // distance = clamp(distance, 0.0f, 50.0f);

    get_sensor_values(sensor_data, &distance);
    // printf("distance in main = %f\n", distance);
    
    // Lissage EMA
    float alpha = 0.05f;  // rapide mais fluide
    
    pthread_mutex_lock(sensor_data->avg_lock);
    sensor_data->interp += alpha;
    if (sensor_data->interp > 1.0f)
        sensor_data->interp = 1.0f;
    smoothed_distance = lerp(sensor_data->last_value, sensor_data->next_value, sensor_data->interp);
    pthread_mutex_unlock(sensor_data->avg_lock);
    distance = smoothed_distance;
    // smoothed_distance = lerp(smoothed_distance, distance, alpha);
    // float maxStep = 1.0f;
    // float diff = distance - smoothed_distance;
    // if (fabsf(diff) > maxStep) {
        // diff = (diff > 0 ? 1 : -1) * maxStep;
        // }
        // smoothed_distance += diff * 0.5f; // ou 1.0f si tu veux du step strict
        
        // Normalisation pour visuel
    float normalized = normalize_value(smoothed_distance, 0.0f, 50.0f);
    float curved = powf(normalized, 1.2f); // légère courbe pour douceur
    
    pixellization = curved * 12.0f;
        
    printf("di = %f interp = %f last = %f next = %f pix = %f\n", distance, sensor_data->interp, sensor_data->last_value, sensor_data->next_value, pixellization);
    // printf("pixellization : %f\n", pixellization);

    radial_gradient(sensor_data->window);
    frame_time = get_time_elapsed(&timer);
    if (target_frame_time_ms < frame_time)
        usleep((target_frame_time_ms - frame_time) * 1000);

    t_img *img = init_img(sensor_data->window);
    for (int i = 0; i < G_HEIGHT; i++)
    {
        for (int j = 0; j < G_WIDTH; j++)
        {
            // if (i % 2 != 0)
                // draw_cell(img, j, i, leds[i][19 - j]);  // c'est pas degueu d'imprimer cmme ca en mode reel
            // else
                draw_cell(img, j, i, leds[i][j]);
        }
        // draw_cell(img, j, i, leds[i][j]);
    }
    // print_matrix(leds);
    // int led_i;
    // for (int i = 0; i < G_HEIGHT; i++)
    // {
    //     for (int j = 0; j < G_WIDTH; j++)
    //     {
    //         if (i % 2 != 0)
    //         {
    //             led_data[led_i * 3 + 0] = get_r(leds[i][19 - j]);
    //             led_data[led_i * 3 + 1] = get_g(leds[i][19 - j]);
    //             led_data[led_i * 3 + 1] = get_b(leds[i][19 - j]);
    //         }
    //         else
    //         {
    //             led_data[led_i * 3 + 0] = get_r(leds[i][j]);
    //             led_data[led_i * 3 + 1] = get_g(leds[i][j]);
    //             led_data[led_i * 3 + 1] = get_b(leds[i][j]);
    //         }
    //     }
    //     draw_cell(img, j, i, leds[i][j]);
    // }
    push_img(img, sensor_data->window);
    frame++;
    return (0); 
}

typedef struct s_params
{
    t_mlx   *window;
}   t_params;

// void update_average_distance(sensor_data_t *sensor_data)
// {
//     static float smoothed = -1.0f;
//     float new_reading = read_sensor_data(sensor_data->uart_fd);

//     if (new_reading == -1)
//         new_reading = 0;

//     // Filtrage exponentiel
//     float alpha = 0.3f;  // Plus petit = plus lissé
//     if (smoothed == -1.0f)
//         smoothed = new_reading;
//     else
//         smoothed = smoothed * (1 - alpha) + new_reading * alpha;

//     // Clamp
//     smoothed = clamp(smoothed, 0.0f, 50.0f);

//     // Envoi dans le buffer partagé
//     pthread_mutex_lock(sensor_data->avg_lock);
//     *(sensor_data->average) = smoothed;
//     pthread_mutex_unlock(sensor_data->avg_lock);
// }

// void update_average_distance(sensor_data_t *sensor_data)
// {
//     static float distance_tab[5] = {-1};
//     float        dist_avg;

//     get_distance_tab(sensor_data->uart_fd, distance_tab, 10);
//     for (int i = 0; i < 5; i++)
//         printf("dist %d = %f\n", i, distance_tab[i]);
//     printf("=========\n");
//     dist_avg = get_avg_ftab(distance_tab, 5);
//     dist_avg = clamp(dist_avg, 0.0f, 100.0f);
//     // dist_avg = norm_value(dist_avg, 0, 50.0f);
//     printf("dist avg = %f\n", dist_avg);
//     pthread_mutex_lock(sensor_data->avg_lock);
//     *(sensor_data->average) = dist_avg;
//     pthread_mutex_unlock(sensor_data->avg_lock);
// }

// void update_average_distance(sensor_data_t *sensor_data)
// {
//     float new_value = read_sensor_data(sensor_data->uart_fd);
//     if (new_value == -1)
//         new_value = 0;

//     pthread_mutex_lock(sensor_data->avg_lock);
//     *(sensor_data->average) = new_value;
//     pthread_mutex_unlock(sensor_data->avg_lock);
// }

// void update_average_distance(sensor_data_t *sensor_data)
// {
//     static float tab[5] = {-1};
//     float new_val = read_sensor_data(sensor_data->uart_fd);
//     if (new_val == -1) new_val = 0;

//     if (tab[0] == -1)
//         for (int i = 0; i < 5; i++) tab[i] = new_val;
//     else {
//         for (int i = 0; i < 4; i++) tab[i] = tab[i + 1];
//         tab[4] = new_val;
//     }

//     float avg = get_avg_ftab(tab, 5);
//     pthread_mutex_lock(sensor_data->avg_lock);
//     *(sensor_data->average) = avg;
//     pthread_mutex_unlock(sensor_data->avg_lock);
// }

void update_average_distance(sensor_data_t *data)
{
    float new_sample = read_sensor_data(data->uart_fd);
    pthread_mutex_lock(data->avg_lock);
    data->average = new_sample;
    pthread_mutex_unlock(data->avg_lock);
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


// void distance_thread_routine(void *data)
// {
//     sensor_data_t *sensor_data;

//     sensor_data = (sensor_data_t *)data;
//     while (1)
//     {
//         update_average_distance(sensor_data);
//         // usleep(10 * 1000);
//         // usleep(10);
//     }
// }

int radial_loop(t_mlx *window)
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
    mlx_loop_hook(window->mlx_ptr, &start_radial, data);
    mlx_loop(window->mlx_ptr);
    return (0);
}