//
// Created by ashna on 11/9/18.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int counter = 20;
int socket_second = 0;

struct SocketNumbers{
    int soc_id;
    struct SocketNumbers *next_sock;
};

struct SemaphoreList{
    int sem_id;
    int value;
    struct SocketNumbers socketNumbers;

    struct SemaphoreList *next;
};

void push(struct SemaphoreList** head_ref, int semid, int value, struct SocketNumbers* sock_head)
{
    struct SemaphoreList* new_node = (struct SemaphoreList*) malloc(sizeof(struct SemaphoreList));
    new_node->sem_id  = semid;
    new_node->value  = value;
    new_node->socketNumbers = *sock_head;
    new_node->next = NULL;

    struct SemaphoreList* last = (*head_ref);
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

void printList(struct SemaphoreList *head)
{
    // printf("\n printing linked list:::\n");
    struct SemaphoreList *node = head->next;
    while (node != NULL)
    {
        printf(" sem_id: %d, valueS: %d \n ", node->sem_id, node->value);
        node = node->next;
    }
}

int create_semaphore(struct SemaphoreList** head){
    printf("in server semaphore");

    push(head, counter, 1, NULL);

    return counter++;


}

int semaphore_p(int semaphore_id){

}

int semaphore_v(int semaphore_id){

}

int semaphore_destroy(int semaphore_id, struct SemaphoreList* head){

}


void infinite_chat(int socket_number){

    char buffer[64];
    int result =0;

    struct SemaphoreList* head = NULL;

    while(1){

        memset(buffer, 0 , sizeof(buffer));

        //read message from client and copy in buffer
        read(socket_number, buffer, sizeof(buffer));

        switch (buffer[0]){
            case 's':
                result = create_semaphore(&head);
                break;
            case 'p':
                result = semaphore_p(buffer[1]);
                break;
            case 'v':
                result = semaphore_v(buffer[1]);
                break;
            case 'd':
                result = semaphore_destroy(buffer[1],&head);
                break;
            default:
                break;
        }
        //printf("From Client: %s",buffer);
    }
}




int main(int argc, char *argv[]){


    int listenport = 1234;
    if(argc > 1){
        listenport = atoi(argv[1]);
    }

    //Create a socket
    int socket_first = socket(AF_INET, SOCK_STREAM,0);
    if(socket_first < 0){
        perror("Cannot create a socket");
        exit(1);
    }

    //Fill address structure containig self address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(listenport);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    //Bind socket to address
    int bind_result = bind(socket_first, (struct sockaddr*) &server_address, sizeof(server_address));
    if(bind_result < 0){
        perror("Cannot bind a socket");
        exit(1);
    }

    //Listen for connection
    int listen_result = listen(socket_first, 1);  //this should be or more?????
    if(listen_result < 0){
        perror("Cannot listen");
        exit(1);
    }

    //Accept a connection
    struct sockaddr_in client_address;
    socklen_t client_address_len;

    socket_second = accept(socket_first, (struct sockaddr*) &client_address, &client_address_len);
    if(socket_second < 0){
        perror("Cannot accept");
        exit(1);
    }
    infinite_chat(socket_second);
    close(socket_first);
}

