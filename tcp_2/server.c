#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"

#define MAX 1000
#define CONNECTIONS 5

void open_new_connections(int clientSocketDescriptor, int socketDescriptor)
{
    if (fork() == 0)
    {
        char serverResponse[MAX];
        char clientMessage[MAX];
        while (1)
        {
            recv(clientSocketDescriptor, &clientMessage, sizeof(clientMessage), 0);
            printf("\nMensagem recebida do cliente %d: %s\n", clientSocketDescriptor, clientMessage);
            printf("\nDigite a mensagem a ser enviada para o cliente %d:\n", clientSocketDescriptor);
            fgets(serverResponse, 10000, stdin);
            send(clientSocketDescriptor, serverResponse, sizeof(serverResponse), 0);
            printf("Mensagem enviada ao cliente %d, caso haja outro cliente em standby digite uma mensagem:\n", clientSocketDescriptor);
        }
    }
    else
    {
        clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
        open_new_connections(clientSocketDescriptor, socketDescriptor);
    }
}

int main()
{
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0), socketClose;

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9002);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(socketDescriptor, CONNECTIONS);

    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    open_new_connections(clientSocketDescriptor, socketDescriptor);

    return 0;
}