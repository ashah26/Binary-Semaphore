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


struct SocketList{
    int soc_id;
    struct SocketList *next_sock;
};

struct SemaphoreList{
    int sem_id;
    int value;
    struct SocketList *socketHead;

    struct SemaphoreList *next;
};

void push(struct  SemaphoreList** head, int semid, int value)
{
//    struct SemaphoreList* new_node = (struct SemaphoreList*) malloc(sizeof(struct SemaphoreList));
//    new_node->sem_id  = semid;
//    new_node->value  = value;
//    new_node->socketNumbers = sock_head;
//    new_node->next = NULL;
//    if(head==NULL){
//        head = new_node;
//        (head)->next = NULL;
//    } else{
//        struct SemaphoreList* last = head;
//        while (last->next != NULL)
//            last = last->next;
//        last->next = new_node;
        struct SemaphoreList* new_node = (struct SemaphoreList*) malloc(sizeof(struct SemaphoreList));
        new_node->sem_id  = counter;
        new_node->value  = 1;
        new_node->socketHead = NULL;
        new_node->next = *head;
        *head = new_node;

//    }
}


void pushSocket(struct SemaphoreList** node, int socketNumber)
{
    struct SocketList* new_socket = (struct SocketList*) malloc(sizeof(struct SocketList));
    new_socket->soc_id = socketNumber;
    new_socket->next_sock = (*node)->socketHead;
    (*node)->socketHead = new_socket;


}

struct SemaphoreList* findSemaphore(struct SemaphoreList* head, int id){
    struct SemaphoreList* node = head;
    while(node!=NULL){
        if(node->sem_id == id){
            return node;
        }
        node = node->next;
    }
    return NULL;
}

int releaseSocket(struct SemaphoreList** node){
    if((*node)->socketHead != NULL){
        struct SocketList* current_node = (*node)->socketHead;
        if(current_node->next_sock == NULL){
            (*node)->socketHead = NULL;
        }else{
            current_node = current_node->next_sock;
            struct SocketList* previous = (*node)->socketHead;
            while (current_node->next_sock  != NULL){
                previous = previous->next_sock;
                current_node = current_node->next_sock;
            }
            previous->next_sock = NULL;
            free(current_node);
        }
    }else{
        return 0;
    }

    return 1;
}

void printSemaphoreList(struct SemaphoreList* head)
{
    // printf("\n printing linked list:::\n");
    struct SemaphoreList *node = head;
    if(node == NULL){
        printf("Semaphore List is empty\n ");
    }else{
        while (node != NULL)
        {
            printf(" sem_id: %d, valueS: %d \n ", node->sem_id, node->value);
            node = node->next;
        }
    }

}

void printSocketList(struct SocketList* head, struct SemaphoreList* semHead)
{
    // printf("\n printing linked list:::\n");
    struct SocketList *node = head;
    if(node == NULL){
        printf("SocketList is empty sem_id: %d  value: %d \n " ,semHead->sem_id, semHead->value);
    }else{
        while (node != NULL)
        {
            printf(" socketNumber: %d sem_id: %d  value: %d\n ", node->soc_id, semHead->sem_id, semHead->value);
            node = node->next_sock;
        }
    }

}


int create_semaphore(struct SemaphoreList** head){
//    printf("in server semaphore\n");

    //TODO: Error checking
    push(head, counter, 1);
    return counter++;
}

int semaphore_p(struct SemaphoreList** head, int semaphore_id, int socketNumber){

    //TODO: error checking
    printf("Request from sem id: %d, for socketNumber: %d \n",semaphore_id,socketNumber );
    struct SemaphoreList* node =  findSemaphore(*head, semaphore_id);
    if(node != NULL){
        if(node-> value == 1){
            node->value = 0;
        }
        pushSocket(&node, socketNumber);
    }else{
        return  -1;
    }
    printSocketList(node->socketHead, node);
    return 0;

}

int semaphore_v(struct SemaphoreList** head, int semaphore_id, int socketNumber){

    //TODO: Error checking
    printf("Release: Request from sem id: %d\n",semaphore_id );
    struct SemaphoreList* node =  findSemaphore(*head, semaphore_id);
    if(node != NULL){
        if(node->value == 1){
            printf("Semaphore is available to use!! \n");
        }else{
            releaseSocket(&node);
            if(node->socketHead == NULL){
                node->value = 1;
            }
        }

    }else{
        return  -1;
    }

    printSocketList(node->socketHead, node);
    return 0;


}

int semaphore_destroy(int semaphore_id){

}



int main(int argc, char *argv[]){


    struct SemaphoreList* head = NULL;

    int listenport = 8081;
    printf("Surver is running on %d\n", listenport);
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

    char buffer[64];
    int buffer_count;
    int result = 0;
    int counttemp = 100;


    while(counttemp > 0){
        struct sockaddr_in client_address;
        socklen_t client_address_len;
        int socket_second;
        socket_second = accept(socket_first, (struct sockaddr*) &client_address, &client_address_len);
        if(socket_second < 0){
            perror("Cannot accept");
            exit(1);
        }

        buffer_count = 0;
//        memset(buffer, 0 , sizeof(buffer));

        //buffer to read message from buffer
        bzero(buffer, sizeof(buffer));

        //read message from client and copy in buffer
        read(socket_second, buffer, sizeof(buffer));
        printf("Message from Client: %c\n", buffer[0]);

        switch (buffer[0]){
            case 's':
                result = create_semaphore(&head);
                //buffer to write message from server
                bzero(buffer, sizeof(buffer));

                buffer[buffer_count++] = result;
                write(socket_second, buffer,sizeof(buffer));

//                printf("s: %d\n", result);
                break;
            case 'p':
                result = semaphore_p(&head, buffer[1], socket_second);
//                printf("p: %d\n", result);
                break;
            case 'v':
                result = semaphore_v(&head, buffer[1], socket_second);
//                printf("v: %d\n", result);
                break;
            case 'd':
                result = semaphore_destroy(buffer[1]);
//                printf("d: %d\n", result);
                break;
            default:
                break;
        }
        counttemp--;
    }




        close(socket_first);
    }



