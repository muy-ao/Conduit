#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "socket.h"

struct client_sock {
    int sock_fd;
    int inbuf;
    char buf[BUF_SIZE];
    struct client_sock *next;
    struct client_sock *prev;
};

int add_client(int sock_fd);
void remove_client(struct client_sock *socket);
struct client_sock* get_head(void);
int get_message(char **dst, struct client_sock *client);
int get_sock_fd(struct client_sock *client);
struct client_sock* get_next(struct client_sock *client);

#endif
