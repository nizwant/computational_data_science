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