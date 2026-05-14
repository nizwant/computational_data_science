#include "../shared_patterns.h"

#define LOCAL_PORT 2137

void print_message(const struct sockaddr_in *src, const char *message) {
    char ip[INET_ADDRSTRLEN];
    char time_str[16];

    // metadata creation
    inet_ntop(AF_INET, &src->sin_addr, ip, sizeof(ip));

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(time_str, sizeof(time_str), "%H:%M", tm_info);

    printf("[%s:%d; %s] %s\n",
           ip,
           ntohs(src->sin_port),
           time_str,
           message);

    fflush(stdout);
}



int setup_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(LOCAL_PORT)
    };

    if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    return fd;
}

int main() {

    int fd = setup_socket();
    if (fd < 0) {
        return 1;
    }

    printf("Listening on UDP port %d...\n", LOCAL_PORT);

    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        if (select(fd + 1, &rfds, NULL, NULL, NULL) < 0) {
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

            PacketHeader *header = (PacketHeader *)buf;

            switch (header->type) {
                case INIT:
                    printf("INIT Packet received\n");
                    break;
                case MESSAGE:  {
                    Packet *packet = (Packet *)buf;
                    char message[MAX_MESS_SIZE];
                    strncpy(message, packet->message, sizeof(message) - 1);
                    message[sizeof(message) - 1] = '\0';

                    print_message(&src, message);
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
