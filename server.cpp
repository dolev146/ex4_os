#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include "myqueue.hpp"
#include "mystack.hpp"
#include <signal.h>

#define SERVERPORT 5004
#define BUFSIZE 1024
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define THREAD_POOL_SIZE 20

pthread_t thread_pool[THREAD_POOL_SIZE];

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;

char client_message[1024];
char size_message[1024];
int server_socket;
void *handle_connection(void *p_client_socket);
int check(int exp, const char *msg);
void *thread_function(void *arg);

void ctrlc_handler(int num)
{
    close(server_socket);
    printf("server socket finish  %d", server_socket);
}

int main(int argc, char **argv)
{
    signal(SIGINT, ctrlc_handler);

    int client_socket, addr_size;
    SA_IN server_addr, client_addr;

    // first off we create a bunch of threads
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);
    }

    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Failed to craete socket");

    // initialize the adress struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVERPORT);

    check(bind(server_socket, (SA *)&server_addr, sizeof(server_addr)), "Bind Failed!");
    check(listen(server_socket, SERVER_BACKLOG), "Listen Failed!");

    while (true)
    {
        printf("Waiting for connections... \n");
        // wait for and eventually accept an incomming connection

        addr_size = sizeof(SA_IN);
        check(client_socket = accept(server_socket, (SA *)&client_addr, (socklen_t *)&addr_size), "accept failed");
        printf("connected! on socket %d \n", client_socket);

        // do whatever we do with connections.
        int *pclient = (int *)malloc(sizeof(int));
        *pclient = client_socket;
        pthread_mutex_lock(&mutex);
        enqueue(pclient);
        pthread_cond_signal(&condition_var);
        pthread_mutex_unlock(&mutex);
    }
    close(server_socket);
    printf("server socket finish  %d", server_socket);
}

void *thread_function(void *arg)
{
    while (true)
    {
        // mutex and cond are designed to work with each other so it is writen like that so that a mutex will not block
        // if contition doesnt met
        int *pclient;
        pthread_mutex_lock(&mutex);
        if ((pclient = dequeue()) == NULL)
        {
            pthread_cond_wait(&condition_var, &mutex);
            // try again
            pclient = dequeue();
        }
        pthread_mutex_unlock(&mutex);
        if (pclient != NULL)
        {
            // we have a connection
            handle_connection(pclient);
        }
    }
}

int check(int exp, const char *msg)
{
    if (exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }
    return exp;
}

void *handle_connection(void *p_client_socket)
{
    int client_socket = *((int *)p_client_socket);
    free(p_client_socket);
    /*
               m.lock();
                if (strncmp(buf, "PUSH", 4) == 0)
                {
                    printf("from client : %s", buf);
                    std::string str_buff = std::string(buf);
                    my_stack.push(str_buff);
                }
                else if (strncmp(buf, "POP", 3) == 0)
                {
                    printf("from client : %s \n", buf);
                    std::string output = my_stack.pop();
                    strncpy(buf, output.c_str(), sizeof(output));
                    send(new_fd, buf, sizeof(buf), 0);
                }
                else if (strncmp(buf, "TOP", 3) == 0)
                {
                    printf("from client : %s \n", buf);
                    std::string output = my_stack.top();
                    strncpy(buf, output.c_str(), sizeof(output));
                    send(new_fd, buf, sizeof(buf), 0);
                }
                else if (strncmp(buf, "size", 4) == 0)
                {
                    printf("from client : %s \n", buf);
                    int output = my_stack.getsize();
                    bzero(buf, sizeof(buf));
                    sprintf(buf, "%d", output);
                    send(new_fd, buf, sizeof(buf), 0);
                }
                else if (strncmp(buf, "exit", 4) == 0 || strncmp(buf, "EXIT", 4) == 0)
                {
                    printf("closed fd : %d \n", new_fd);
                    close(new_fd);
                    m.unlock();
                    return NULL;
                }
                m.unlock();
    */
    pthread_mutex_lock(&mutex);
    if (recv(client_socket, client_message, sizeof(client_message), 0) == -1)
    {
        perror("recv");
    }

    if (strncmp(client_message, "PUSH", 4) == 0)
    {
        printf("from client : %s", client_message);
        push(client_message);
        printf("push good!");
    }
    else if (strncmp(client_message, "POP", 3) == 0)
    {
        printf("from client : %s \n", client_message);
        pop();
        printf("pop good!");
    }
    else if (strncmp(client_message, "TOP", 3) == 0)
    {
        printf("from client : %s \n", client_message);
        char *msg = top();
        send(client_socket, msg, sizeof(msg), 0);
        free(msg);
        bzero(client_message, sizeof(client_message));
    }
    else if (strncmp(client_message, "size", 4) == 0)
    {
        printf("from client : %s \n", client_message);
        int output = get_size();
        bzero(client_message, sizeof(client_message));
        strcat(client_message, "DEBUG:");
        sprintf(size_message, "%d", output);
        strncat(client_message, size_message, sizeof(size_message));
        send(client_socket, client_message, sizeof(client_message), 0);
        bzero(client_message, sizeof(client_message));
    }
    pthread_mutex_unlock(&mutex);

    if (strncmp(client_message, "hello from ruby \n", 17) == 0) /* hello from ruby \n */
    {
        char buffer_ruby_test[BUFSIZE] = "hi from server ";
        printf("%s", client_message);
        send(client_socket, buffer_ruby_test, 1024, 0);
        close(client_socket);
    }

    return NULL;
    // Dzone
    // int newSocket = client_socket;
    // recv(newSocket, client_message, 1024, 0);

    // printf("%s", client_message);
    // char *firstcli = &client_message[0];

    // // Send message to the client socket

    // // pthread_mutex_lock(&lock);
    // char *message = malloc(sizeof(client_message) + 20);
    // strcpy(message, "Hello Client : ");
    // strcat(message, firstcli);
    // strcat(message, "\n");
    // strcpy(buffer, message);
    // free(message);
    // // pthread_mutex_unlock(&lock);
    // sleep(1);
    // send(newSocket, buffer, 1000, 0);
    // printf("Exit socket number %d \n", newSocket);
    // close(newSocket);
    // pthread_exit(NULL);
    // Dzone
}
