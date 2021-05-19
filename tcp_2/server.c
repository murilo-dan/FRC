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
    if (fork() > 0)
    {
        //o pai(primeiro cliente) cria novas conexoes para o servidor caso um novo cliente tente se conectar
        clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
        //a funcao de abrir novas conexoes e aberta novamente, ela e recursiva
        open_new_connections(clientSocketDescriptor, socketDescriptor);
    }
    else
    {
        char serverResponse[MAX];
        char clientMessage[MAX];
        while (1)
        {   
            //servidor recebe mensagem de um dos clientes
            recv(clientSocketDescriptor, &clientMessage, sizeof(clientMessage), 0);
            //o cliente que enviou a mensagem e especificado
            printf("\nMensagem recebida do cliente %d: %s\n", clientSocketDescriptor, clientMessage);
            //servidor envia resposta ao cliente especifico
            printf("\nDigite a mensagem a ser enviada para o cliente %d:\n", clientSocketDescriptor);
            fgets(serverResponse, 10000, stdin);
            send(clientSocketDescriptor, serverResponse, sizeof(serverResponse), 0);
            //caso hajam outros clientes na espera de resposta o servidor pode enviar mensagens para eles tambem
            printf("\nMensagem enviada ao cliente %d, caso haja outro cliente em standby digite a resposta para ele:\n", clientSocketDescriptor);
        }
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

    //servidor aguarda novas conexoes
    listen(socketDescriptor, CONNECTIONS);

    //servidor recebe tentativa de conexao do cliente
    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    //servidor adiciona o cliente a conexoes estabelecidas
    open_new_connections(clientSocketDescriptor, socketDescriptor);

    return 0;
}