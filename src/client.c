#include "client.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <username> <password>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    const char *password = argv[2];

    // create socket and send init message
    struct sockaddr_in server = {0};
    int fd = initialize_client(username, password, &server);
    if (fd < 0)
        return 1;

    // add create dummy client
    struct sockaddr_in client_dummy_socket = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY)};

    // initialize hashmap
    Client *clients_hashmap = NULL;
    add_user_to_hashmap(&clients_hashmap, username, password, client_dummy_socket);
    add_user_to_hashmap(&clients_hashmap, SERVER_USERNAME, SERVER_USERNAME, server);
    int received_init_response = 0;

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
                    send_get_user(fd, get_username, get_password, &server);
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

                    send_message(fd, temp, peer->ip_addr, peer->port);
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

                    send_ping(fd, target->ip_addr, target->port);
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
