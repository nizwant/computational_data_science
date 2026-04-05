#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server;
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(2137);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 3);

    printf("Waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);

    read(client_fd, buffer, sizeof(buffer));
    printf("Received: %s\n", buffer);

    close(client_fd);
    close(server_fd);

    return 0;
}