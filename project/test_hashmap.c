#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>


#include "uthash.h"


typedef struct _client{
    char username[32];
    struct in_addr ip_addr;
    uint16_t port;
    time_t last_time_seen;
    char password[32]; //SHA-256 of password
    char public_key[32]; // ED25519 key used as a authentication method 
    UT_hash_handle hh;
} Client;

void add_user(Client **clients, char *username, struct in_addr ip, uint16_t port) {
    Client *s;

    s = malloc(sizeof *s);
    strcpy(s->username, username);
    s->last_time_seen = time(NULL);
    s->ip_addr = ip;
    s->port = port;
    HASH_ADD_STR(*clients, username, s); 
}


int main(){

    Client *clients = NULL;

    struct in_addr ip;
    inet_pton(AF_INET, "192.168.0.22", &ip);
    add_user(&clients, "nizwan1", ip, 3333);


    inet_pton(AF_INET, "192.168.0.23", &ip);
    add_user(&clients, "nizwan2", ip, 4444);

    inet_pton(AF_INET, "192.168.0.24", &ip);
    add_user(&clients, "nizwan3", ip, 5555);

    

    Client *c;

    HASH_FIND_STR(clients, "nizwan2", c);

    printf("%d\n", c->port); // prints 4444

    return 1;
}