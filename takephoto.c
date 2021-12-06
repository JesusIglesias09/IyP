#include <stdio.h>   // fprintf()
#include <unistd.h>  // fork(), exec()
#include <string.h>  // strerror()
#include <errno.h>   // errno
#include <stdlib.h>  // exit()
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8000
#define IPADDRESS "192.168.1.9"
#define IMGNEEDED 4

int main(int argc, const char *argv[])
{
    uint32_t sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return 0;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, IPADDRESS, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return 0;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return 0;
    }
    for(uint32_t count=0;count<IMGNEEDED;count++){
        system("raspistill -n -o image.jpg");

        //Get pic size
    	FILE *picture;
    	picture = fopen("image.jpg", "r");
        uint32_t size;
    	fseek(picture, 0, SEEK_END);
    	size = ftell(picture);
    	fseek(picture, 0, SEEK_SET);

        //Send pic size
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
    }
}
