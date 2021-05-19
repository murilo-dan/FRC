#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/mman.h>
#include "netdb.h"
#include "arpa/inet.h"
#include <errno.h>

#define MAX 1000
#define CONNECTIONS 5

void *create_shared_memory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    return mmap(NULL, size, protection, visibility, -1, 0);
}

void send_to_all(void *shmem, char *clientMessage)
{
    int *addresses = (int *)malloc(sizeof(int) * 100);
    for (int j = 0; j < 100; j++)
    {
        addresses[j] = 0;
    }
    addresses = (int *)shmem;
}

void open_new_connections(int clientSocketDescriptor, int socketDescriptor, void *shmem, int *addresses, int i)
{
    if (fork() > 0)
    {
        addresses[i] = clientSocketDescriptor;
        i++;

        memcpy(shmem, addresses, sizeof(int) * 100);

        clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);
        open_new_connections(clientSocketDescriptor, socketDescriptor, shmem, addresses, i);
    }
    else
    {
        usleep(200000);
        char serverResponse[MAX];
        char clientMessage[MAX];
        while (1)
        {
            recv(clientSocketDescriptor, &clientMessage, sizeof(clientMessage), 0);
            printf("\nMensagem recebida do cliente %d: %s\n", clientSocketDescriptor, clientMessage);
            printf("\nEnviando mensagem para os clientes conectados.\n");
            send_to_all(shmem, clientMessage);
            printf("\nMensagem enviada.\n");
        }
    }
}

int main()
{
    int i = 0;
    int addresses[100] = {0};

    void *shmem = create_shared_memory(sizeof(int) * 100);

    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9002);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(socketDescriptor, CONNECTIONS);

    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    open_new_connections(clientSocketDescriptor, socketDescriptor, shmem, addresses, i);

    return 0;
}