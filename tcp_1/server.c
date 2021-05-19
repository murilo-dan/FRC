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

    //servidor aguarda conexao de algum cliente
    listen(socketDescriptor, BACKLOG);

    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
    printf("\nConexao com novo cliente estabelecida.\n");

    while (1)
    {
        //servidor aguarda o cliente enviar uma mensagem
        recv(clientSocketDescriptor, &clientMessage, sizeof(clientMessage), 0);
        printf("\nMensagem recebida do cliente: %s\n", clientMessage);
        //caso a mensagem do cliente seja uma mensagem de parada(stop) o servidor ira seguir o procedimento de fechar a conexao
        if (strcmp(clientMessage, "stop") == 0)
        {   
            printf("Conexao com cliente fechada, aguardando novas conexões\n");
            //servidor aguarda um novo cliente apos fechamento de conexao
            clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
        }
        //caso seja uma mensagem normal o servidor ira responder o cliente (half-duplex)
        else
        {
            printf("\nDigite a mensagem a ser enviada para o cliente:\n");
            fgets(serverResponse, 10000, stdin);   
            //servidor envia mensagem para o cliente
            send(clientSocketDescriptor, serverResponse, sizeof(serverResponse), 0);
            printf("Mensagem enviada ao cliente\n");
        }
    }
    return 0;
}