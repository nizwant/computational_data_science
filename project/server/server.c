#include "../shared_patterns.h"


int add_user(Client **clients_hashmap, char *username, char* password, struct sockaddr_in src) {
    Client *s;

    HASH_FIND_STR(*clients_hashmap, username, s);
    if (s != NULL) {
      return -1;
    }

    s = malloc(sizeof *s);
    strcpy(s->username, username);
    strcpy(s->password, password);
    s->last_time_seen = time(NULL);
    s->ip_addr = src.sin_addr;
    s->port = ntohs(src.sin_port);
    HASH_ADD_STR(*clients_hashmap, username, s); 
    return 1;
}

int main() {

    // initialize hashmap
    Client *clients_hashmap = NULL;

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
            char buf[sizeof(MetadataPacket)];
            struct sockaddr_in src;
            socklen_t slen = sizeof(src);

            ssize_t n = recvfrom(fd, buf, sizeof(buf) - 1, 0,
                                 (struct sockaddr *)&src, &slen);
            if (n < 0) {
                perror("recvfrom");
                continue;
            }

            PacketHeader *header = (PacketHeader *)buf;
            int type = header->type; 
            char username[32];
            strcpy(username, header->sender_username);

            switch (type) {
                case INIT:
                    printf("INIT Packet received\n");

                    InitPacket *packet = (InitPacket *)buf;
                    char password[32];
                    strncpy(password, packet->password, sizeof(password) - 1);

                    if (add_user(&clients_hashmap, username, password, src) < 0){
                        printf("user already exist\n"); // TODO SEND ERROR TO USER
                        break;
                    }

                    PacketHeader init_r = {INIT_RESPONSE, "server"};
                    //TODO create struct and fill it with necessary date
                    if (sendto(fd, &init_r, sizeof(init_r), 0, (struct sockaddr *)&src, sizeof(src)) < 0) {
                        perror("sendto");
                    }
                    break;

                case PING:
                    printf("PING Packet received\n");
                    Client *s;

                    HASH_FIND_STR(clients_hashmap, username, s);
                    if (s == NULL) {
                        printf("user that PINGed doesn't exist in hashmap");
                        break;
                    }
                    s->last_time_seen = time(NULL);
                    break;

                case GET_PEER:
                    printf("GET_PEER Packet received\n");

                    Client *sb;
                    HASH_FIND_STR(clients_hashmap, "nizwan", sb); //TODO replace with fetching username from message
                    if (sb == NULL) {
                        printf("GET_PEER user doesn't exist in hashmap");
                        break;
                    }

                    //TODO Validate if password hash matches

                    // send info to requester
                    PacketHeader start_pinging_r = {START_PINGING_PEER, "server"};
                    //TODO create struct and fill it with necessary date 
                    if (sendto(fd, &start_pinging_r, sizeof(start_pinging_r), 0, (struct sockaddr *)&src, sizeof(src)) < 0) {
                        perror("sendto");
                    }

                    // send START_PINGING_PEER to requested

                    struct sockaddr_in peer = {0};
                    peer.sin_family = AF_INET;
                    peer.sin_port = htons(sb->port);
                    peer.sin_addr = sb->ip_addr;

                    //TODO create struct and fill it with necessary date
                    if (sendto(fd, &start_pinging_r, sizeof(start_pinging_r), 0, (struct sockaddr *)&peer, sizeof(peer)) < 0) {
                        perror("sendto");
                    }

                    break;

                default:
                    printf("Unknown packet type for server: %d\n", type);
                    break;
            }
        }

    }

    close(fd);
    return 0;
}
