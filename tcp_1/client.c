#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"

#define h_addr h_addr_list[0]

#define PORT 9002
#define MAX 1000

int main()
{
    char serverResponse[MAX];
    char clientMessage[MAX];
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0), socketClose;

    char hostname[MAX], ipaddress[MAX];
    struct hostent *hostIP;
    if (gethostname(hostname, sizeof(hostname)) == 0)
    {
        hostIP = gethostbyname(hostname);
    }
    else
    {
        printf("ERROR:FCC4539 IP Address Not ");
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    //cliente tenta se conectar ao servidor
    printf("\nConexao com servidor estabelecida\n");

    while (1)
    {
        //cliente decide o que enviar, caso não haja mais informações para enviar o sinal de parada (stop) e enviado
        printf("\nDigite mensagem para enviar ao servidor(Envie stop caso não haja mais dados para enviar):\n");
        scanf("%s", clientMessage);
        //cliente envia a mensagem
        send(socketDescriptor, clientMessage, sizeof(clientMessage), 0);
        if (strcmp(clientMessage, "stop") == 0)
        {
            //caso a mensagem seja de parada o cliente fecha a conexao com o servidor
            printf("Finalizando conexão.\n");
            socketClose = close(socketDescriptor);
            exit(0);
        }
        printf("Mensagem enviada, aguardando retorno do servidor\n");
        //cliente aguarda resposta do servidor
        recv(socketDescriptor, serverResponse, sizeof(serverResponse), 0);
        printf("\nMensagem recebida do servidor: %s\n", serverResponse);
    }
    return 0;
}