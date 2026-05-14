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

                    InitPacket *init_packet = (InitPacket *)buf;
                    char password[32];
                    strncpy(password, init_packet->password, sizeof(password) - 1);

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

                    GetPeerPacket *packet = (GetPeerPacket *)buf;
                    char dest_username[32];
                    char dest_password[32];
                    strncpy(dest_username, packet->username, sizeof(dest_username) - 1);
                    strncpy(dest_password, packet->password, sizeof(dest_password) - 1);
                    
                    Client *sb;
                    HASH_FIND_STR(clients_hashmap, dest_username, sb);
                    if (sb == NULL) {
                        printf("GET_PEER user doesn't exist in hashmap");
                        break;
                    }

                    if (strcmp(sb->password, dest_password) != 0){
                        printf("Password doesn't match");
                        break;
                    }

                    // send info to requester
                    PacketHeader start_pinging_r = {START_PINGING_PEER, "server"};

                    // create StartPingingPeerPacket
                    StartPingingPeerPacket start_packet_source = {0};
                    start_packet_source.header = start_pinging_r;
                    strncpy(start_packet_source.username, dest_username, sizeof(start_packet_source.username) - 1);
                    start_packet_source.ip = sb->ip_addr;
                    start_packet_source.port = sb->port;

                    if (sendto(fd, &start_packet_source, sizeof(start_packet_source), 0, (struct sockaddr *)&src, sizeof(src)) < 0) {
                        perror("sendto");
                    }

                    // send START_PINGING_PEER to requested

                    struct sockaddr_in peer = {0};
                    peer.sin_family = AF_INET;
                    peer.sin_port = htons(sb->port);
                    peer.sin_addr = sb->ip_addr;

                    // create StartPingingPeerPacket
                    StartPingingPeerPacket start_packet_dest = {0};
                    start_packet_dest.header = start_pinging_r;
                    strncpy(start_packet_dest.username, username, sizeof(start_packet_dest.username) - 1);
                    start_packet_dest.ip = src.sin_addr;
                    start_packet_dest.port = src.sin_port;
                    
                    if (sendto(fd, &start_packet_dest, sizeof(start_packet_dest), 0, (struct sockaddr *)&peer, sizeof(peer)) < 0) {
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
