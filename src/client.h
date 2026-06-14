#ifndef CLIENT_H
#define CLIENT_H

#include "shared_patterns.h"

int initialize_client(const char *username, const char *password, struct sockaddr_in *server)
{

    int fd = setup_socket(0);
    if (fd < 0)
    {
        return -1;
    }

    server->sin_family = AF_INET;
    server->sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server->sin_addr) != 1)
    {
        fprintf(stderr, "bad server IP\n");
        close(fd);
        return -1;
    }

    PacketHeader header = {0};
    header.type = INIT;
    strncpy(header.sender_username, username, sizeof(header.sender_username) - 1);

    InitPacket init_packet = {0};
    init_packet.header = header;
    strncpy(init_packet.password, password, sizeof(init_packet.password) - 1);

    if (sendto(fd, &init_packet, sizeof(init_packet), 0, (struct sockaddr *)server, sizeof(*server)) < 0)
    {
        perror("sendto");
    }

    return fd;
}

#endif
