#include "../shared_patterns.h"

#define LOCAL_PORT 2137


int setup_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in local = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(LOCAL_PORT)
    };

    if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(fd);
        return 1;
    }

    return fd;
}

int main() {

    int fd = setup_socket();
    if (fd < 0) {
        return 1;
    }

    printf("Listening on UDP port %d...\n", LOCAL_PORT);

    for (;;) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        FD_SET(STDIN_FILENO, &rfds);

        int maxfd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;

        int r = select(maxfd, &rfds, NULL, NULL, NULL);
        if (r < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(fd, &rfds)) {
            char buf[sizeof(Packet)];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            PacketHeader *header = (PacketHeader*)buf;

            switch (header->type) {
                case INIT:
                    printf("type 1 received\n");
                    break;
                case MESSAGE:  {
                    Packet *pkt = (Packet*)buf;
                    char message[MAX_MESS_SIZE];
                    strncpy(message, pkt->message, sizeof(message) - 1);
                    message[sizeof(message) - 1] = '\0';

                    // metadata creation
                    char src_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &src.sin_addr, src_ip, sizeof(src_ip));

                    time_t rawtime;
                    struct tm * timeinfo;
                    char time_buffer[80];

                    time ( &rawtime );
                    timeinfo = localtime ( &rawtime );
                    strftime(time_buffer, sizeof(time_buffer), "%H:%M", timeinfo);

                    printf("[%s:%d; %s] %s\n", src_ip, ntohs(src.sin_port), time_buffer, message);
                    fflush(stdout);
                    break;
                }
                default:
                    printf("Unknown packet type: %d\n", header->type);
                    break;
            }
        }

    }

    close(fd);
    return 0;
}
