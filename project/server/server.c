#include "../shared_patterns.h"

int main() {

    // initialize hashmap
    Client *clients = NULL;

    int fd = setup_socket(1);
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

                    PacketHeader init_r = {INIT_RESPONSE, "server"};
                    if (sendto(fd, &init_r, sizeof(init_r), 0, (struct sockaddr *)&src, sizeof(src)) < 0) {
                        perror("sendto");
                    }
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
