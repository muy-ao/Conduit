#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <netinet/in.h>

#define MAX_BACKLOG 5
#define BUF_SIZE 1024

struct listen_sock {
    int sock_fd;
    struct sockaddr_in *addr;
};

struct client_sock;

void setup_listen_socket(struct listen_sock *passive);
int accept_connection(int fd);
int read_from_client(struct client_sock *client);
int find_CRLF(char *buf, int inbuf);

#endif
