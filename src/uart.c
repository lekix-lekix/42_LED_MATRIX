#include "../include/include.h"

// Configuration du port série
int configure_serial_port(int fd) {
    struct termios options;

    // Récupérer les paramètres actuels du port série
    if (tcgetattr(fd, &options) < 0) 
    {
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
