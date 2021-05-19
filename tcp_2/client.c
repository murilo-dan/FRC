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

    //cliente tenta se conectar ao servidor
    connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    printf("\nConexao com servidor estabelecida\n");

    while (1)
    {
        //cliente decide o que enviar
        printf("\nDigite mensagem para enviar ao servidor:\n");
        scanf("%s", clientMessage);
        //cliente envia a mensagem
        send(socketDescriptor, clientMessage, sizeof(clientMessage), 0);
        printf("\nMensagem enviada, aguardando retorno do servidor\n");
        //cliente aguarda resposta do servidor
        recv(socketDescriptor, serverResponse, sizeof(serverResponse), 0);
        printf("\nMensagem recebida do servidor: %s\n", serverResponse);
    }
    return 0;
}