// Client side C/C++ program to demonstrate Socket programming

#include <stdio.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <string.h>

#define PORT 8000

   

int main(int argc, char const *argv[])

{

    

    ///////////////////STRUCT FOR DATA///////////////////////////

    struct packID 

    {

        char SOF;

        char SENSOR;

        char DATA_SIZE;

        char DATA;

        uint32_t checksum;

    }IDPACKED;

    /////////////////////////////////////////////////////////////

    

    int sock = 0;

    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};

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

    

    //////////////////////////DATA FROM STRUCT//////////////////////////

    IDPACKED.SOF = 0xFF;

    IDPACKED.SENSOR = 0xAA;

    IDPACKED.DATA_SIZE = 0x01;

    IDPACKED.DATA = 0x55;

    IDPACKED.checksum = IDPACKED.SOF + IDPACKED.SENSOR + IDPACKED.DATA_SIZE + IDPACKED.DATA;

    ////////////////////////////////////////////////////////////////////

    

    send(sock , &IDPACKED , sizeof(IDPACKED) , 0 );

    printf("Hello message sent\n");


    printf("%s\n",buffer );

    return 0;

}
