#include <stdlib.h>
#include <string.h>

#include "client.h"

struct client_sock *head = NULL;
struct client_sock *tail = NULL;


// 1 fail 0 succ
int add_client(int sock_fd) {

    struct client_sock *client = malloc(sizeof(struct client_sock));
    if (client == NULL) {
        return 1;
    }

    client->next = NULL;
    client->prev = NULL;
    client->sock_fd = sock_fd;
    client->inbuf = 0;
    memset(client->buf, 0, BUF_SIZE);

    if (head == NULL) {
        head = client;
        tail = client;
    } else {
        client->prev = tail;
        tail->next = client;
        tail = client;
    }
    return 0;
}


void remove_client(struct client_sock *client) {

    if (client == head && client == tail) {
        head = NULL;
        tail = NULL;

    } else if (client == head) {
        head = head->next;
        head->prev = NULL;

    } else if (client == tail) {
        tail = tail->prev;
        tail->next = NULL;

    } else {
        client->prev->next = client->next;
        client->next->prev = client->prev;
    }

    free(client);
}

struct client_sock* get_head() {
    return head;
} 

int get_message(char **dst, struct client_sock *client) {

    int *inbuf = &(client->inbuf);
    char *buf = client->buf;
    
    int ind = find_CRLF(buf, *inbuf);
    if (ind == -1) {
        return 1;
    }

    int count = ind - 2;
    char *msg = malloc(count + 1);
    if (msg == NULL) {
        return 1;
    }

    memcpy(msg, buf, count);
    msg[count] = '\0';
    *dst = msg;
    memmove(buf, buf + ind, *inbuf - ind);
    *inbuf -= ind;

    return 0;
}

int get_sock_fd(struct client_sock *client) {
    return client->sock_fd;
}

struct client_sock* get_next(struct client_sock *client) {
    return client->next;
}

