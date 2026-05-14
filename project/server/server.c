#include "../shared_patterns.h"

int main() {

    int local_port = 2137;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(local_port);

    if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(fd);
        return 1;
    }


    printf("Ready to listen.\n");
    printf("Local port: %d\n", local_port);

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
            char buf[MAX_MESS_SIZE];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            PacketHeader *hdr = (PacketHeader*)buf;

            switch (hdr->type) {
                case INIT:
                    printf("type 1 received\n");
                    break;
                case MESSAGE:  {
                    Packet *pkt = (Packet*)buf;
                    char message[1024];
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
            }
        }

        fflush(stdout);
    }

    close(fd);
    return 0;
}
