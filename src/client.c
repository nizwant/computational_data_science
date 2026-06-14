#include "protocol.h"
#include "client_lib.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "usage: %s <username> <password>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    const char *password = argv[2];

    if (peer_connect(username, password) < 0)
        return 1;

    int fd = peer_get_fd();

    DEBUG_PRINT("Connecting to server on: %s:%d\n", SERVER_IP, SERVER_PORT);

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
            char user_input[MAX_MESS_SIZE];
            if (!fgets(user_input, sizeof(user_input), stdin))
            {
                break; // EOF
            }

            size_t len = strlen(user_input);
            if (len > 0 && user_input[len - 1] == '\n')
            {
                user_input[len - 1] = '\0';
                len--;
            }

            if (strcmp(user_input, "/quit") == 0)
            {
                break;
            }
            else if (strncmp(user_input, "/get_user", 9) == 0)
            {
                char get_username[32];
                char get_password[32];

                if (sscanf(user_input, "/get_user %31s %31s", get_username, get_password) == 2)
                {
                    peer_get_user(get_username, get_password);
                }
                else
                {
                    printf("bad format, expected format: /get_user username password\n");
                }
            }
            else if (strncmp(user_input, "/message", 8) == 0)
            {

                char receiver_username[32];
                char message[MAX_MESS_SIZE];

                if (sscanf(user_input, "/message %31s %[^\n]", receiver_username, message) == 2)
                {
                    if (peer_send_message(receiver_username, message) < 0)
                    {
                        printf("peer '%s' not found, use /get_user first\n", receiver_username);
                    }
                }
                else
                {
                    printf("bad format, expected format: /message username message\n");
                }
            }

            else if (strncmp(user_input, "/ping", 5) == 0)
            {
                char ping_target[32];
                if (sscanf(user_input, "/ping %31s", ping_target) == 1)
                {
                    if (peer_send_ping(ping_target) < 0)
                    {
                        printf("peer '%s' not found, use /get_user first\n", ping_target);
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
            peer_receive(0);
        }

        printf("> ");
        fflush(stdout);
    }

    peer_disconnect();
    return 0;
}
