#include "../shared_patterns.h"

int setup_listening_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(SERVER_PORT)
    };

    if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    return fd;
}

int main() {

    // initialize hashmap
    Client *clients = NULL;

    int fd = setup_listening_socket();
    if (fd < 0) {
        return 1;
    }

    printf("Listening on UDP port %d...\n", SERVER_PORT);

    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        if (select(fd + 1, &rfds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(fd, &rfds)) {
            char buf[sizeof(PacketHeader)];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            PacketHeader *header = (PacketHeader *)buf;

            switch (header->type) {
                case INIT:
                    printf("INIT Packet received\n");
                    break;

                case PING:
                    printf("PING Packet received\n");
                    break;

                case GET_PEER:
                    printf("GET_PEER Packet received\n");
                    break;

                default:
                    printf("Unknown packet type for server: %d\n", header->type);
                    break;
            }
        }

    }

    close(fd);
    return 0;
}
