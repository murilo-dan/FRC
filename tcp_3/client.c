#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"
#include <signal.h>

#define h_addr h_addr_list[0]

#define PORT 9002
#define MAX 1000

void signal_handler()
{
    printf("Sinal ativado\n");
    usleep(500000);
    signal(SIGUSR1, signal_handler);
}

int main()
{
    pid_t cpid;
    char clientResponse[MAX];
    char clientMessage[MAX];
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

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
    printf("\nConexao com servidor estabelecida\n");

    if ((cpid = fork()))
    {
        usleep(100000);
        while (1)
        {
            printf("Esperando mensagem\n");
            recv(socketDescriptor, clientResponse, sizeof(clientResponse), 0);
            kill(cpid, SIGUSR1);
            printf("\nMensagem recebida dos clientes: %s\n", clientResponse);
        }
    }
    else
    {
        signal(SIGUSR1, signal_handler);
        while (1)
        {
            usleep(200000);
            printf("\nDigite mensagem para enviar ao servidor:\n");
            scanf("%s", clientMessage);
            send(socketDescriptor, clientMessage, sizeof(clientMessage), 0);
            printf("Mensagem enviada.\n");
            usleep(200000);
        }
    }
    return 0;
}