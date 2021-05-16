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
#define BACKLOG 5

int main()
{
    char serverResponse[MAX];
    char clientMessage[MAX];
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0), socketClose;

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9002);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(socketDescriptor, BACKLOG);

    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    while (1)
    {
        recv(clientSocketDescriptor, &clientMessage, sizeof(clientMessage), 0);
        printf("\nMensagem recebida do cliente: %s\n", clientMessage);
        if (strcmp(clientMessage, "stop") == 0)
        {
            printf("Conexao com cliente fechada, aguardando novas conexões\n");
            clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
        }
        else
        {
            printf("\nDigite a mensagem a ser enviada para o cliente:\n");
            fgets(serverResponse, 10000, stdin);
            send(clientSocketDescriptor, serverResponse, sizeof(serverResponse), 0);
            printf("Mensagem enviada ao cliente\n");
        }
    }
    return 0;
}