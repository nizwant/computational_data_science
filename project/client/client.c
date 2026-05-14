#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>

#define MAX_MESS_SIZE 1024

typedef enum _packet_type {
    INIT,
    INIT_RESPONSE,
    PING,
    GET_PEER,
    GET_PEER_RESPONSE,
    START_PINGING_PEER,
    MESSAGE
} PacketType;

typedef struct _packet_header{
    PacketType type;
    char my_username[32];
} PacketHeader;

typedef struct _packet{
    PacketHeader header;
    char message[1024];
} Packet;

int main() {

    const char *peer_ip = "127.0.0.1";
    int peer_port = 2137;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in local = {0};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);

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

    PacketHeader header = {INIT, "nizwan"};
    if (sendto(fd, &header, sizeof(header), 0,
                       (struct sockaddr *)&peer, sizeof(peer)) < 0) {
                perror("sendto");
            }

    PacketHeader header2 = {MESSAGE, "nizwan"};
    Packet p = {header2, "aaaaaaab"};
    if (sendto(fd, &p, sizeof(p), 0,
                       (struct sockaddr *)&peer, sizeof(peer)) < 0) {
                perror("sendto");
            }

    printf("Ready.\n");
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
            char message[MAX_MESS_SIZE];
            if (!fgets(message, sizeof(message), stdin)) {
                break; // EOF
            }

            size_t len = strlen(message);
            if (len > 0 && message[len - 1] == '\n') {
                message[len - 1] = '\0';
                len--;
            }

            if (strcmp(message, "/quit") == 0) {
                break;
            }

            Packet p2;
            p2.header = header2;

            strncpy(p2.message, message, sizeof(p2.message) - 1);
            p2.message[sizeof(p2.message) - 1] = '\0';


            if (sendto(fd, &p2, sizeof(p2), 0,
                            (struct sockaddr *)&peer, sizeof(peer)) < 0) {
                        perror("sendto");
                    }


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
