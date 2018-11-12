//
// Created by ashna on 11/9/18.
//


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>


int sem_create();
int sem_P(int sem_id);
int sem_V(int sem_id);
int sem_destroy(int sem_id);


int main() {
    int mutex1, mutex2; //semaphores
    int pid; // process ID
    if ((mutex1 = sem_create()) < 0) {
        printf("Cannot create semaphore mutex1.\n");
        _exit(1);
    }  //if

    if ((mutex2 = sem_create()) < 0) {
        _exit(2);
        printf("Cannot create semaphore mutex2.\n");
    }  // if

    // basic checks
    sem_P(mutex2);
    printf("Completed a P() operation on mutex2.\n");
    sem_V(mutex2);
    sem_V(mutex2);
    printf("Completed two V() operations on mutex2.\n");

    if ((pid = fork()) == 0) {
        // child process
        printf("Child process requests mutex1.\n");
        sem_P(mutex1);
        printf("Child process got mutex1.\n");
        sleep(10);
        printf("Child process is ready to release mutex1.\n");
        sem_V(mutex1);
        printf("Child process released mutex1.\n");
        _exit(0);
    } // if
    sleep(3);
    printf("Parent process requests mutex1.\n");
    sem_P(mutex1);
    printf("Parent process got mutex1.\n");
    sleep(8);
    printf("Parent process is ready to release mutex1.\n");
    sem_V(mutex1);
    printf("Parent process released mutex1.\n");

    // Ending
    sem_destroy(mutex1);
    if (sem_P(mutex1) >= 0) {
        printf("OOPS! Semaphore mutex1 was not properly destroyed!\n");
    } // if
    sem_destroy(mutex2);
    if (sem_P(mutex2) >= 0) {
        printf("OOPS! Semaphore mutex2 was not properly destroyed!\n");
    } //if
    if (sem_P(mutex1) >= 0) {
        printf("OOPS! Server accepted a call to a non-existent semaphore!\n");
    }
} // main



int sem_create() {

    //Create a socket
    int socket_first = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_first < 0){
        perror("Cannot create socket");
        exit(1);
    }

    //Fill the address of server
    struct sockaddr_in client_address;
    int client_address_len;
    memset(&client_address, 0, sizeof(client_address));
    char* peerHost = "localhost";

    //resolve server address(convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL){
        perror("Cannot define host address");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    short peerPort = 8081;
    client_address.sin_port = htons(peerPort);

    //Write a resolved IP address of server to address structure
    memmove(&(client_address.sin_addr.s_addr), host->h_addr_list[0],4);

    //Connect to remote server
    int remote = connect(socket_first, (struct sockaddr*) &client_address, sizeof(client_address));
    if (remote < 0){
        perror("Cannot connect");
        exit(1);
    }

    //write a message in buffer and accordingly perform operations
    char buffer[64];
    bzero(buffer, sizeof(buffer));
    buffer[0] = 's';
    buffer[1] = 0;
    write(socket_first, buffer, sizeof(buffer));

    //initialize a buffer to read response from server
    bzero(buffer, sizeof(buffer));
    read(socket_first,buffer, sizeof(buffer));

   //close the socket
    close(socket_first);
    return  buffer[0];

    // Should return a sem_id if call succeeded and -1 otherwise
}

int sem_P(int sem_id) {


    //Create a socket
    int socket_first = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_first < 0){
        perror("Cannot create socket");
        exit(1);
    }

    //Fill the address of server
    struct sockaddr_in client_address;
    int client_address_len;
    memset(&client_address, 0, sizeof(client_address));
    char* peerHost = "localhost";

    //resolve server address(convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL){
        perror("Cannot define host address");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    short peerPort = 8081;
    client_address.sin_port = htons(peerPort);

    //Write a resolved IP address of server to address structure
    memmove(&(client_address.sin_addr.s_addr), host->h_addr_list[0],4);

    //Connect to remote server
    int remote = connect(socket_first, (struct sockaddr*) &client_address, sizeof(client_address));
    if (remote < 0){
        perror("Cannot connect");
        exit(1);
    }

    //write a message in buffer and accordingly perform operations
    char buffer[64];
    bzero(buffer, sizeof(buffer));
    buffer[0] = 'p';
    buffer[1] = sem_id;
    write(socket_first, buffer, sizeof(buffer));

    //initialize a buffer to read response from server
    bzero(buffer, sizeof(buffer));
    read(socket_first,buffer, sizeof(buffer));

    //close the socket
    close(socket_first);
    return buffer[0];

    // Should return 0 if call succeeded and -1 otherwise
}

int sem_V(int sem_id) {


    //Create a socket
    int socket_first = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_first < 0){
        perror("Cannot create socket");
        exit(1);
    }

    //Fill the address of server
    struct sockaddr_in client_address;
    int client_address_len;
    memset(&client_address, 0, sizeof(client_address));
    char* peerHost = "localhost";

    //resolve server address(convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL){
        perror("Cannot define host address");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    short peerPort = 8081;
    client_address.sin_port = htons(peerPort);

    //Write a resolved IP address of server to address structure
    memmove(&(client_address.sin_addr.s_addr), host->h_addr_list[0],4);

    //Connect to remote server
    int remote = connect(socket_first, (struct sockaddr*) &client_address, sizeof(client_address));
    if (remote < 0){
        perror("Cannot connect");
        exit(1);
    }

    //write a message in buffer and accordingly perform operations
    char buffer[64];
    bzero(buffer, sizeof(buffer));
    buffer[0] = 'v';
    buffer[1] = sem_id;
    write(socket_first, buffer, sizeof(buffer));

    //initialize a buffer to read response from server
    bzero(buffer, sizeof(buffer));
    read(socket_first,buffer, sizeof(buffer));

    //close the socket
    close(socket_first);
    return buffer[0];

    // Should return 0 if call succeeded and -1 otherwise
}

int sem_destroy(int sem_id) {

    //Create a socket
    int socket_first = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_first < 0){
        perror("Cannot create socket");
        exit(1);
    }

    //Fill the address of server
    struct sockaddr_in client_address;
    int client_address_len;
    memset(&client_address, 0, sizeof(client_address));
    char* peerHost = "localhost";

    //resolve server address(convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL){
        perror("Cannot define host address");
        exit(1);
    }

    client_address.sin_family = AF_INET;
    short peerPort = 8081;
    client_address.sin_port = htons(peerPort);

    //Write a resolved IP address of server to address structure
    memmove(&(client_address.sin_addr.s_addr), host->h_addr_list[0],4);

    //Connect to remote server
    int remote = connect(socket_first, (struct sockaddr*) &client_address, sizeof(client_address));
    if (remote < 0){
        perror("Cannot connect");
        exit(1);
    }

    //write a message in buffer and accordingly perform operations
    char buffer[64];
    bzero(buffer, sizeof(buffer));
    buffer[0] = 'd';
    buffer[1] = sem_id;
    write(socket_first, buffer, sizeof(buffer));

    //initialize a buffer to read response from server
    bzero(buffer, sizeof(buffer));
    read(socket_first,buffer, sizeof(buffer));

    //close the socket
    close(socket_first);
    return buffer[0];

    // Should return 0 if call succeeded and -1 otherwise
}



