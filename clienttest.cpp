#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <pthread.h>

void *cientThread(void *arg)
{
    printf("In thread\n");
    char message[1000];

    char buffer[1024];
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;

    // Create the socket.
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address
    // Address family is Internet
    serverAddr.sin_family = AF_INET;

    // Set port number, using htons function
    serverAddr.sin_port = htons(5007);

    // Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Connect the socket to the server using the address
    addr_size = sizeof serverAddr;
    connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
    strcpy(message, "PUSH a");

    for (int i = 0; i < 100; i++)
    {
        if (send(clientSocket, message, strlen(message), 0) < 0)
        {
            printf("Send failed\n");
        }
        usleep(5000);
    }
    printf("finished pushing 100 a\n");
    bzero(message, sizeof(message));

    strcpy(message, "size");
    // printf("%s\n",message);

    if (send(clientSocket, message, strlen(message), 0) < 0)
    {
        printf("Send failed\n");
    }
    // Read the message from the server into the buffer
    if (recv(clientSocket, buffer, 1024, 0) < 0)
    {
        printf("Receive failed\n");
    }
    // Print the received message
    printf("Data received: size is %s\n", buffer);
    close(clientSocket);
    pthread_exit(NULL);
}
int main()
{
    int i = 0;
    pthread_t tid[2];
    while (i < 2)
    {
        if (pthread_create(&tid[i], NULL, cientThread, NULL) != 0)
            printf("Failed to create thread\n");
        i++;
    }
    sleep(20);
    i = 0;
    while (i < 2)
    {
        pthread_join(tid[i++], NULL);
        printf("%d:\n", i);
    }
    return 0;
}