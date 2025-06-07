#include "../include/include.h"

void update_average_distance(sensor_data_t *data)
{
    float new_sample = read_sensor_data(data->uart_fd);
    pthread_mutex_lock(data->avg_lock);
    data->average = new_sample;
    pthread_mutex_unlock(data->avg_lock);
}

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