#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>

#define BUF_SZ 1024

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <local_port> <peer_ip> <peer_port>\n", argv[0]);
        return 1;
    }

    int local_port = atoi(argv[1]);
    const char *peer_ip = argv[2];
    int peer_port = atoi(argv[3]);

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

    struct sockaddr_in peer = {0};
    peer.sin_family = AF_INET;
    peer.sin_port = htons(peer_port);
    if (inet_pton(AF_INET, peer_ip, &peer.sin_addr) != 1) {
        fprintf(stderr, "bad peer ip\n");
        close(fd);
        return 1;
    }

    printf("Ready.\n");
    printf("Local port: %d\n", local_port);
    printf("Peer: %s:%d\n", peer_ip, peer_port);
    printf("Type messages and press Enter.\n\n");

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

        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            char buf[BUF_SZ];
            if (!fgets(buf, sizeof(buf), stdin)) {
                break; // EOF
            }

            size_t len = strlen(buf);
            if (len > 0 && buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
                len--;
            }

            if (strcmp(buf, "/quit") == 0) {
                break;
            }

            if (sendto(fd, buf, len, 0,
                       (struct sockaddr *)&peer, sizeof(peer)) < 0) {
                perror("sendto");
            }
        }

        if (FD_ISSET(fd, &rfds)) {
            char buf[BUF_SZ];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            buf[n] = '\0';

            char src_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &src.sin_addr, src_ip, sizeof(src_ip));

            time_t rawtime;
            struct tm * timeinfo;
            char buffer[80];

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);

            printf("\n[%s:%d; %s] %s\n", src_ip, ntohs(src.sin_port), buffer, buf);
            printf("> ");
            fflush(stdout);
        }

        printf("> ");
        fflush(stdout);
    }

    close(fd);
    return 0;
}
