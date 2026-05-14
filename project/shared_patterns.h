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
    GET_PEER_RESPONSE,
    START_PINGING_PEER,
    MESSAGE
} PacketType;

typedef struct _packet_header{
    PacketType type;
    char sender_username[32];
} PacketHeader;

typedef struct _packet{
    PacketHeader header;
    char message[MAX_MESS_SIZE];
} Packet;

typedef struct _client{
    char username[32];
    struct in_addr ip_addr;
    uint16_t port;
    time_t last_time_seen;
    char password[32]; //SHA-256 of password
    char public_key[32]; // ED25519 key used as a authentication method 
    UT_hash_handle hh;
} Client;