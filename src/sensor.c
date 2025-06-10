#include "../include/include.h"

void update_average_distance(sensor_data_t *data)
{
    int sample1;
    int sample2;

    read_sensor_data(data->uart_fd, &sample1, &sample2);
    printf("sample 1 = %d sample2 = %d\n", sample1, sample2);
    pthread_mutex_lock(data->dist_lock);
    data->dist_sensor_1 = sample1;
    data->dist_sensor_2 = sample2;
    pthread_mutex_unlock(data->dist_lock);
}

void get_sensor_1_values(sensor_data_t *data, float *curr_distance)
{
    pthread_mutex_lock(data->dist_lock);
    float dist_1 = clamp(data->dist_sensor_1, 0.0f, 100.0f);
    // float dist_2 = clamp(data->dist_sensor_1, 0.0f, 100.0f);
    if (*curr_distance == -1)
    {
        *curr_distance = dist_1;
        pthread_mutex_unlock(data->dist_lock);
        return ;
    }
    if (dist_1 == *curr_distance)
    {
        pthread_mutex_unlock(data->dist_lock);
        return ;
    }
    data->sens_1_last_value = *curr_distance;
    data->sens_1_next_value = dist_1;
    data->sens_1_interp = 0.0f;
    *curr_distance = dist_1;
    pthread_mutex_unlock(data->dist_lock);
}

void get_sensor_2_values(sensor_data_t *data, float *curr_distance)
{
    pthread_mutex_lock(data->dist_lock);
    // float dist_2 = clamp(data->dist_sensor_2, 0.0f, 5.0f);
    float dist_2 = clamp(data->dist_sensor_2, 0.0f, 100.0f);
    if (*curr_distance == -1)
    {
        *curr_distance = dist_2;
        pthread_mutex_unlock(data->dist_lock);
        return ;
    }
    if (dist_2 == *curr_distance)
    {
        pthread_mutex_unlock(data->dist_lock);
        return ;
    }
    data->sens_1_last_value = *curr_distance;
    data->sens_1_next_value = dist_2;
    data->sens_1_interp = 0.0f;
    *curr_distance = dist_2;
    pthread_mutex_unlock(data->dist_lock);
}