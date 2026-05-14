#include "../shared_patterns.h"

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

int main() {
    int fd = setup_socket(0);
    if (fd < 0) {
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) != 1) {
        fprintf(stderr, "bad server IP\n");
        close(fd);
        return 1;
    }

    int received_init_response = 0;
    PacketHeader header = {INIT, "nizwan"};
    if (sendto(fd, &header, sizeof(header), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("sendto");
    }

    printf("Connecting to server on: %s:%d\n", SERVER_IP, SERVER_PORT);

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
            PacketHeader header2 = {MESSAGE, "nizwan"};
            p2.header = header2;

            strncpy(p2.message, message, sizeof(p2.message) - 1);
            p2.message[sizeof(p2.message) - 1] = '\0';


            if (sendto(fd, &p2, sizeof(p2), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
                perror("sendto");
            }


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
                case INIT_RESPONSE:
                    printf("INIT_RESPONSE Packet received\n");
                    received_init_response ++;
                    break;

                case PING:
                    printf("PING Packet received\n");
                    break;

                case GET_PEER_RESPONSE:
                    printf("GET_PEER_RESPONSE Packet received\n");
                    break;

                case START_PINGING_PEER:
                    printf("START_PINGING_PEER Packet received\n");
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
                    printf("Unknown packet type for client: %d\n", header->type);
                    break;
            }
        }

        printf("> ");
        fflush(stdout);
    }

    close(fd);
    return 0;
}
