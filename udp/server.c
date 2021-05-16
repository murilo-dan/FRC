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
    char send_msg[MAX_MSG];
    char recv_msg[MAX_MSG];

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

    client_size = sizeof(client_address);

    while (1)
    {
        memset(recv_msg, 0x0, MAX_MSG);
        memset(send_msg, 0x0, MAX_MSG);

        printf("Aguardando mensagem do cliente...\n\n");

        n = recvfrom(sd, recv_msg, MAX_MSG, 0, (struct sockaddr *)&client_address, &client_size);
        if (n < 0)
        {
            printf("Não foi possível receber dados.\n");
            continue;
        }

        printf("Mensagem recebida: %s\n", recv_msg);

        printf("Digite a resposta para o cliente: ");
        fgets(send_msg, sizeof(send_msg), stdin);

        sendto(sd, send_msg, strlen(send_msg), 0, (struct sockaddr *)&client_address, client_size);

        printf("Resposta enviada ao cliente.\n\n");
    }

    return 0;
}