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

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <username> <password>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    const char *password = argv[2];


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
    PacketHeader header = {0};
    header.type = INIT;
    strncpy(header.sender_username, username, sizeof(header.sender_username) - 1);

    InitPacket init_packet = {0};
    init_packet.header = header;
    strncpy(init_packet.password, password, sizeof(init_packet.password) - 1);

    if (sendto(fd, &init_packet, sizeof(init_packet), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("sendto");
    }

    printf("Connecting to server on: %s:%d\n", SERVER_IP, SERVER_PORT);

    for (;;) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        FD_SET(STDIN_FILENO, &rfds);

        int maxfd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;

        if (select(maxfd, &rfds, NULL, NULL, NULL) < 0) {
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

            MessagePacket p2;
            PacketHeader header2 = {MESSAGE, username};
            p2.header = header2;

            strncpy(p2.message, message, sizeof(p2.message) - 1);
            p2.message[sizeof(p2.message) - 1] = '\0';


            // if (sendto(fd, &p2, sizeof(p2), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
            //     perror("sendto");
            // }


            PacketHeader header3 = {0};
            header3.type = GET_PEER;
            strncpy(header3.sender_username, username, sizeof(header3.sender_username) - 1);

            GetPeerPacket p3;
            p3.header = header3;
            strncpy(p3.username, "nizwan", sizeof(p3.username) - 1);
            strncpy(p3.password, "aaab", sizeof(p3.password) - 1);
            if (sendto(fd, &p3, sizeof(p3), 0, (struct sockaddr *)&server, sizeof(server)) < 0) {
                perror("sendto");
            }

        }

        if (FD_ISSET(fd, &rfds)) {
            char buf[sizeof(MessagePacket)];
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

                case START_PINGING_PEER:
                    printf("START_PINGING_PEER Packet received\n");
                    
                    StartPingingPeerPacket *spp_packet = (StartPingingPeerPacket *)buf;
                    printf("Username: %s\n", spp_packet->username);
                    printf("Port: %d\n", spp_packet->port);

                    break;

                case MESSAGE:  {
                    MessagePacket *packet = (MessagePacket *)buf;
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
