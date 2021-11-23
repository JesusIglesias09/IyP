// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PORT 8000
   
int main(int argc, char const *argv[])
{
       
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "192.168.1.9", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    //Get pic size
    FILE *picture;
    picture = fopen("image.jpg", "r");
    uint32_t size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);

    //Send pic size
    char p_array[size];
    send(sock, &size, sizeof(size),0);
    printf("Size sent\n");

    //Send pic as byte array
    printf("Sending Picture as Byte Array\n");
    char send_buffer[100];
    int nb = fread(send_buffer, 1, sizeof(send_buffer), picture);
    while(!feof(picture)) {
        send(sock, send_buffer, nb, 0);
        nb = fread(send_buffer, 1, sizeof(send_buffer), picture);
    }
    return 0;
}
