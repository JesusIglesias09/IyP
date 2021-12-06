// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#define PORT 8000
#define IMGNEEDED 4
#define MAXSOCKETS   	10   //Limit of sockets.
#define TIMETOSLEEP  	10   //Time in seconds that pthread

//Global variables used by ptheads
int32_t listSocket[MAXSOCKETS];
int32_t socketIndex;          //Socket Index
pthread_mutex_t socketMutex = PTHREAD_MUTEX_INITIALIZER;
int32_t storage_socketID[5];
void *SendMessages (void *ptr);

int main(int argc, char  *argv[])
{

    int32_t server_fd;
    int32_t sinSize;
    int32_t new_socket;
    struct sockaddr_in address;
    pthread_t pthSendMessages;
    int32_t opt = 1;
    int32_t iPthRc;

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

    //It is necessary to create the pthread that will send the messages
    //to the connected sockets every 5 seconds.
    iPthRc = pthread_create(&pthSendMessages, NULL, &SendMessages, NULL);
    if (iPthRc < 0)
    {
        printf("There was an error trying to create the pthread, iPthRc:%d\n", iPthRc);
        exit(1);
    }

    sinSize = sizeof(struct sockaddr_in);
    //From here we have to handle every next connection.
    while (true)
    {
        //This will make the thread to sleep until a new connection
        //comes and then we will send the conection to a new thread
        //to be handle.
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&sinSize);
        //Validate that the accept was success, otherwise we will try again
        if (new_socket < 0)
        {
            printf("There was an error traying to accept in the socket, errno:%d\n", errno);
            continue;
        }

        //Add the socket to the list of sockets in an available pleace.
        pthread_mutex_lock(&socketMutex);
        printf("DEBUG: Thread created, Lock Started\n");

        for(uint8_t count=0;count<IMGNEEDED;count++){
            char prefix[100] = "Image";
            char str[10][2] ={{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"},{"0"}};
            char suffix[] = ".jpg";

            //Read pic size
            int size=0;
            read( new_socket , &size, sizeof(int));
            printf("Image size =  %d\n",size);

            //Read pic byte array
            char p_array[100]="";
            strcat(prefix,str[count]);
            strcat(prefix,suffix);
            FILE *image = fopen(prefix, "w");
            printf("Receiving image\n");
            int nb = read(new_socket, p_array, 100);
            fwrite(p_array, 1, nb, image);
            while (size-ftell(image)>200) {
                nb = read(new_socket, p_array, 100);
                fwrite(p_array, 1, nb, image);
            }
            nb = read(new_socket, p_array, size-ftell(image));
            fwrite(p_array, 1, nb, image);
            fclose(image);
            printf("Image created\n");
        }

        printf("Shuting down a socket of index: %d\n", socketIndex);
        shutdown(listSocket[socketIndex], SHUT_RDWR);
        listSocket[socketIndex] = 0;
        sleep(1);
        pthread_mutex_unlock(&socketMutex);
    }
    return 0;
}

void *SendMessages (void *ptr)
{
    //Variable declaration.
    int32_t bSend_return;
    char count[1];
    count[0] = 'A';
    while (1)
    {
        pthread_mutex_lock(&socketMutex);

        //Message implementation depending on your application
        bSend_return = send(storage_socketID[0], (void*)count , strlen(count), MSG_NOSIGNAL);
        if (bSend_return <= 0)
        {
            printf("No connection\n");
        }
        else
        {
            printf("DATA SENT\n");
        }
        pthread_mutex_unlock(&socketMutex);
        //This pthread must sleep for 10 seconds.
        sleep(TIMETOSLEEP);
    }
    return 0;
}
