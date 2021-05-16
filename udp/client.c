#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MSG 100

int main(int argc, char *argv[])
{
    int sd, rc, n, server_size;
    struct sockaddr_in client_address;
    struct sockaddr_in server_address;
    char send_msg[MAX_MSG];
    char recv_msg[MAX_MSG];

    if (argc < 3)
    {
        printf("Digite o IP e a Porta do servidor.\n");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    client_address.sin_port = htons(0);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        printf("Não foi possível abrir o socket.\n");
        exit(1);
    }

    rc = bind(sd, (struct sockaddr *)&client_address, sizeof(client_address));
    if (rc < 0)
    {
        printf("Não foi possível realizar o bind.\n");
        exit(1);
    }

    server_size = sizeof(server_address);

    while(1)
    {
        memset(recv_msg, 0x0, MAX_MSG);
        memset(send_msg, 0x0, MAX_MSG);

        printf("Digite a mensagem a ser enviada para o servidor: ");
        fgets(send_msg, sizeof(send_msg), stdin);
        rc = sendto(sd, send_msg, strlen(send_msg), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        if (rc < 0)
        {
            printf("Não foi possível enviar os dados.\n");
            close(sd);
            exit(1);
        }
        printf("Enviando mensagem: %s\nAguardando resposta...\n\n", send_msg);

        n = recvfrom(sd, recv_msg, MAX_MSG, 0, (struct sockaddr *)&server_address, &server_size);
        printf("Mensagem recebida do servidor: %s\n", recv_msg);
    }

    return 0;
}