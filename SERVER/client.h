#ifndef _CLIENT_H_
#define _CLIENT_H_

#define BUF_SIZE 1024

int add_socket(int sock_fd);
void rm_socket(struct client_sock *socket);
struct client_sock* get_head(void);












#endif