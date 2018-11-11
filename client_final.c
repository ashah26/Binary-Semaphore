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


//Socket creation and server connection
int socket_create(char input,int id){

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
    short peerPort = 1234;
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
    char buffer[10];
    memset(&buffer, 0, sizeof(buffer));
    buffer[0] = input;
    buffer[1] = id;
    write(socket_first, buffer, sizeof(buffer));



    close(socket_first);



}



int main() {
    int mutex1, mutex2; //semaphores
    int pid; // process ID
    if ((mutex1 = sem_create()) < 0) {
        printf("Cannot create semaphore mutex1.\n");
        _exit(1);
    }  //if
    else{
        printf("Created semaphore 20");
    }
    if ((mutex2 = sem_create()) < 0) {
        _exit(2);
        printf("Cannot create semaphore mutex2.\n");
    }  // if
    else{
        printf("Created semaphore 21");
    }

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


    socket_create('s',0);

    // add your own code
    // Should return a sem_id if call succeeded and -1 otherwise
}

int sem_P(int sem_id) {


    socket_create('p',sem_id);

    // add your own code
    // Should return 0 if call succeeded and -1 otherwise
}

int sem_V(int sem_id) {

    socket_create('v', sem_id);

    // add your own code
    // Should return 0 if call succeeded and -1 otherwise
}

int sem_destroy(int sem_id) {


    socket_create('d', sem_id);

    // add your own code
    // Should return 0 if call succeeded and -1 otherwise
}



