#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>

#include "uthash.h"

#define MAX_MESS_SIZE 1024
#define SERVER_PORT 2137
#define SERVER_IP "127.0.0.1"

typedef enum _packet_type {
    INIT,
    INIT_RESPONSE,
    PING,
    GET_PEER,
    START_PINGING_PEER,
    MESSAGE
} PacketType;

typedef struct _packet_header{
    u_int8_t type;
    char sender_username[32];
} PacketHeader;

typedef struct _init_packet{
    PacketHeader header;
    char password[32];
} InitPacket;

typedef struct _get_peer_packet{
    PacketHeader header;
    char username[32];
    char password[32];
} GetPeerPacket;

typedef struct _start_pinging_peer_packet{
    PacketHeader header;
    char username[32];
    struct in_addr ip;
    uint16_t port;
} StartPingingPeerPacket;

typedef union _metadata_packet{
    InitPacket;
    GetPeerPacket;
    StartPingingPeerPacket;
} MetadataPacket;


typedef struct _message_packet{
    PacketHeader header;
    char message[MAX_MESS_SIZE];
} MessagePacket;

typedef struct _client{
    char username[32];
    struct in_addr ip_addr;
    uint16_t port;
    time_t last_time_seen;
    char password[32]; //SHA-256 of password
    char public_key[32]; // ED25519 key used as a authentication method 
    UT_hash_handle hh;
} Client;

int setup_socket(int server_socket) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in local = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if (server_socket){
        local.sin_port = htons(SERVER_PORT);
    }

    if (bind(fd, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        close(fd);
        return -1;
    }

    return fd;
}

