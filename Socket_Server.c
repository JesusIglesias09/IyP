// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#define PORT 8000

int main(int argc, char  *argv[])
{
    int32_t server_fd;
    int32_t new_socket;
    struct sockaddr_in address;
    int32_t opt = 1;
    int32_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return 1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    printf("PHASE 1\n");
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        return 1;
    }
    printf("PHASE 2\n");
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        return -1;
    }
    printf("PHASE 3\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        return 1;
    }

    printf("ENDING PHASE\n");
    //Read pic size
    int size;
    read( new_socket , &size, sizeof(int));
    printf("Size image =  %d\n",size);

    //Read pic byte array
    printf("Reading Picture Byte Array\n");
    char p_array[100];
    FILE *image = fopen("image.jpg", "w");
    int nb = read(new_socket, p_array, 100);
    while (nb > 0) {
        fwrite(p_array, 1, nb, image);
        nb = read(new_socket, p_array, 100);
    }
    fclose(image);
    printf("Image created");
    return 0;
}
