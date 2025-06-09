#include "../include/include.h"

void update_average_distance(sensor_data_t *data)
{
    char sample1[10];
    char sample2[10];

    bzero(sample1, 10);
    bzero(sample2, 10);
    read_sensor_data(data->uart_fd, sample1, sample2);
    pthread_mutex_lock(data->avg_lock);
    data->dist_sensor_1 = atof(sample1);
    data->dist_sensor_2 = atof(sample1);
    pthread_mutex_unlock(data->avg_lock);
}

void get_sensor_values(sensor_data_t *data, float *curr_distance)
{
    pthread_mutex_lock(data->avg_lock);
    float avg = clamp(data->dist_sensor_1, 0.0f, 100.0f);
    if (*curr_distance == -1)
    {
        *curr_distance = avg;
        pthread_mutex_unlock(data->avg_lock);
        return ;
    }
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