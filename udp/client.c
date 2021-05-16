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
    int sd, rc, i, n;
    struct sockaddr_in client_address;
    struct sockaddr_in server_address;
    char msg[MAX_MSG];

    if (argc < 3)
    {
        printf("uso correto: %s <ip_do_servidor> <porta_do_servidor> <dado1> ... <dadoN> \n", argv[0]);
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
        printf("%s: não pode abrir o socket \n", argv[0]);
        exit(1);
    }

    rc = bind(sd, (struct sockaddr *)&client_address, sizeof(client_address));
    if (rc < 0)
    {
        printf("%s: não pode fazer um bind da porta\n", argv[0]);
        exit(1);
    }
    printf("{UDP, IP_Cli: %s, Porta_Cli: %u, IP_R: %s, Porta_R: %s}\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), argv[1], argv[2]);

    int server_size = sizeof(server_address);
    for (i = 3; i < argc; i++)
    {
        rc = sendto(sd, argv[i], strlen(argv[i]), 0, (struct sockaddr *)&server_address, sizeof(server_address));
        if (rc < 0)
        {
            printf("%s: nao pode enviar dados %d \n", argv[0], i - 1);
            close(sd);
            exit(1);
        }
        printf("Enviando parametro %d: %s\n", i - 2, argv[i]);

        n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&server_address, &server_size);
        printf("%s\n", msg);
    }
    return 0;
}