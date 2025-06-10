#include "../include/include.h"

// Configuration du port série
int configure_serial_port(int fd) {
    struct termios options;
    
    if (tcgetattr(fd, &options) < 0) {
        perror("tcgetattr");
        return -1;
    }
    
    // Vider les buffers AVANT la config
    tcflush(fd, TCIOFLUSH);
    
    // Vitesse
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    
    // 8N1
    options.c_cflag &= ~PARENB;  // Pas de parité
    options.c_cflag &= ~CSTOPB;  // 1 bit d'arrêt
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;      // 8 bits
    
    // Mode raw complet
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY | ICRNL | INLCR);
    options.c_oflag &= ~OPOST;
    
    // Timeout plus court pour detecter les problèmes
    options.c_cc[VMIN] = 0;   // Pas d'attente minimale
    options.c_cc[VTIME] = 5;  // 0.5 seconde timeout
    
    options.c_cflag |= CREAD | CLOCAL;
    
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("tcsetattr");
        return -1;
    }
    
    // Vider à nouveau après config
    tcflush(fd, TCIOFLUSH);
    return 0;
}

void read_sensor_data(int uart_fd, int *sample1, int *sample2)
{
    char buffer[32];
    int bytes_read;
    
    // Vider le buffer au début
    tcflush(uart_fd, TCIFLUSH);
    while (1) 
    {
        bytes_read = read(uart_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null terminator
            break;
        } else if (bytes_read == 0) {
            printf("Timeout\n");
        } else {
            perror("read");
            break;
        }
    }
    char sample1_str[6];
    char sample2_str[6];
    bzero(sample1_str, 6);
    bzero(sample2_str, 6);
    int i = -1;
    int j = -1;
    while (buffer[++i] && buffer[i] != ' ')
        sample1_str[i] = buffer[i];
    while (buffer[++i])
        sample2_str[++j] = buffer[i];
    *sample1 = atoi(sample1_str);
    *sample2 = atoi(sample2_str);
}
