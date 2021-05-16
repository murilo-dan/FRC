#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define MAX_MSG 100

int main(int argc, char *argv[])
{
    int sd, rc, n, client_size;
    struct sockaddr_in client_address;
    struct sockaddr_in server_address;
    char msg[MAX_MSG];

    if (argc < 3)
    {
        printf("Digite IP e Porta para este servidor.\n");
        exit(1);
    }

    if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Não foi possível abrir o socket.\n");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    if ((rc = bind(sd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0))
    {
        printf("Não foi possível realizar o bind.\n");
        exit(1);
    }

    while (1)
    {
        memset(msg, 0x0, MAX_MSG);
        client_size = sizeof(client_address);

        n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&client_address, &client_size);
        if (n < 0)
        {
            printf("%s: não foi possível receber dados.\n", argv[0]);
            continue;
        }

        printf("{UDP, IP_L: %s, Porta_L: %u,", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
        printf(" IP_R: %s, Porta_R: %u}, Mensagem recebida: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), msg);

        sendto(sd, msg, n, 0, (struct sockaddr *)&client_address, client_size);
    }

    return 0;
}