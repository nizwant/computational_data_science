#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server;
    char *message = "siema babka";

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2137);
    server.sin_addr.s_addr = inet_addr("192.168.0.11");

    // 2. Connect to server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // 3. Send data
    send(sock, message, strlen(message), 0);

    printf("Message sent\n");

    // 4. Close socket
    close(sock);

    return 0;
}