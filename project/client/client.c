#include "../shared_patterns.h"

void print_message(const struct sockaddr_in *src, const char *message)
{
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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <username> <password>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    const char *password = argv[2];

    // initialize hashmap
    Client *clients_hashmap = NULL;

    // add server to hashmap

    struct sockaddr_in client_dummy_socket = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY)};

    add_user_to_hashmap(&clients_hashmap, username, password, client_dummy_socket);

    int fd = setup_socket(0);
    if (fd < 0)
    {
        return 1;
    }

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) != 1)
    {
        fprintf(stderr, "bad server IP\n");
        close(fd);
        return 1;
    }
    add_user_to_hashmap(&clients_hashmap, SERVER_USERNAME, "server", server);

    int received_init_response = 0;
    PacketHeader header = {0};
    header.type = INIT;
    strncpy(header.sender_username, username, sizeof(header.sender_username) - 1);

    InitPacket init_packet = {0};
    init_packet.header = header;
    strncpy(init_packet.password, password, sizeof(init_packet.password) - 1);

    if (sendto(fd, &init_packet, sizeof(init_packet), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("sendto");
    }

    printf("Connecting to server on: %s:%d\n", SERVER_IP, SERVER_PORT);

    for (;;)
    {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        FD_SET(STDIN_FILENO, &rfds);

        int maxfd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;

        if (select(maxfd, &rfds, NULL, NULL, NULL) < 0)
        {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &rfds))
        {
            char message[MAX_MESS_SIZE];
            if (!fgets(message, sizeof(message), stdin))
            {
                break; // EOF
            }

            size_t len = strlen(message);
            if (len > 0 && message[len - 1] == '\n')
            {
                message[len - 1] = '\0';
                len--;
            }

            if (strcmp(message, "/quit") == 0)
            {
                break;
            }
            else if (strncmp(message, "/get_user", 9) == 0)
            {
                char get_username[32];
                char get_password[32];

                if (sscanf(message, "/get_user %31s %31s", get_username, get_password) == 2)
                {
                    // success
                    header.type = GET_PEER;

                    GetPeerPacket p3;
                    p3.header = header;
                    strncpy(p3.username, get_username, sizeof(p3.username) - 1);
                    strncpy(p3.password, get_password, sizeof(p3.password) - 1);
                    if (sendto(fd, &p3, sizeof(p3), 0, (struct sockaddr *)&server, sizeof(server)) < 0)
                    {
                        perror("sendto");
                    }
                }
                else
                {
                    printf("bad format, expected format: /get_user username password\n");
                }
            }
            else if (strncmp(message, "/message", 8) == 0)
            {

                char message_username[32];
                char temp[MAX_MESS_SIZE];

                if (sscanf(message, "/message %31s %[^\n]", message_username, temp) == 2)
                {
                    Client *peer;
                    HASH_FIND_STR(clients_hashmap, message_username, peer);
                    if (peer == NULL)
                    {
                        printf("peer '%s' not found, use /get_user first\n", message_username);
                        continue;
                    }

                    struct sockaddr_in peer_addr = {0};
                    peer_addr.sin_family = AF_INET;
                    peer_addr.sin_port = htons(peer->port);
                    peer_addr.sin_addr = peer->ip_addr;

                    header.type = MESSAGE;

                    MessagePacket p2;
                    p2.header = header;
                    strncpy(p2.message, temp, sizeof(p2.message) - 1);
                    p2.message[sizeof(p2.message) - 1] = '\0';

                    if (sendto(fd, &p2, sizeof(p2), 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0)
                    {
                        perror("sendto");
                    }
                }
                else
                {
                    printf("bad format, expected format: /message username message\n");
                }
            }

            else if (strncmp(message, "/ping", 5) == 0)
            {
                char ping_target[32];
                if (sscanf(message, "/ping %31s", ping_target) == 1)
                {
                    Client *target;
                    HASH_FIND_STR(clients_hashmap, ping_target, target);
                    if (target == NULL)
                    {
                        printf("peer '%s' not found, use /get_user first\n", ping_target);
                        continue;
                    }

                    struct sockaddr_in target_addr = {0};
                    target_addr.sin_family = AF_INET;
                    target_addr.sin_port = htons(target->port);
                    target_addr.sin_addr = target->ip_addr;

                    header.type = PING;
                    if (sendto(fd, &header, sizeof(header), 0, (struct sockaddr *)&target_addr, sizeof(target_addr)) < 0)
                    {
                        perror("sendto");
                    }
                }
                else
                {
                    printf("bad format, expected format: /ping username\n");
                }
            }

            else
            {
                printf("bad format expected one of those:\n - /quit\n - /get_user username password\n - /ping username\n - /message username message\n");
            }
        }

        if (FD_ISSET(fd, &rfds))
        {
            char buf[sizeof(MessagePacket)];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0)
            {
                perror("recvfrom");
                continue;
            }

            PacketHeader *header = (PacketHeader *)buf;

            switch (header->type)
            {
            case INIT_RESPONSE:
                printf("INIT_RESPONSE Packet received\n");
                received_init_response++;
                break;

            case PING:
                printf("PING Packet received\n");
                break;

            case START_PINGING_PEER:
            {
                printf("START_PINGING_PEER Packet received\n");

                StartPingingPeerPacket *spp_packet = (StartPingingPeerPacket *)buf;
                printf("Username: %s\n", spp_packet->username);
                printf("Port: %d\n", spp_packet->port);

                struct sockaddr_in peer_addr = {0};
                peer_addr.sin_family = AF_INET;
                peer_addr.sin_port = htons(spp_packet->port);
                peer_addr.sin_addr = spp_packet->ip;

                add_user_to_hashmap(&clients_hashmap, spp_packet->username, "", peer_addr);

                char peer_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &spp_packet->ip, peer_ip, sizeof(peer_ip));
                printf("Peer %s added (%s:%d)\n", spp_packet->username, peer_ip, spp_packet->port);

                break;
            }

            case MESSAGE:
            {
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
