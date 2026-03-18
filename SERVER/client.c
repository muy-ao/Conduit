
#include "client.h"

struct client_sock {
    int sock_fd;
    int inbuf;
    char buf[BUF_SIZE];
    struct client_sock *next;
    struct client_sock *prev;
};

struct client_sock *head = NULL;
struct client_sock *tail = NULL;


// 1 fail 0 succ
int add_socket(int sock_fd) {

    struct client_sock *socket = malloc(sizeof(struct client_sock));
    if (socket == NULL) {
        return 1;
    }

    socket->next = NULL;
    socket->prev = NULL;
    socket->sock_fd = sock_fd;
    socket->inbuf = 0;
    memset(socket->buf, 0, BUF_SIZE);

    if (head == NULL) {
        head = socket;
        tail = socket;
    } else {
        socket->prev = tail;
        tail->next = socket;
        tail = socket;
    }
    return 0;
}


void rm_socket(struct client_sock *socket) {

    if (socket == head && socket == tail) {
        head = NULL;
        tail = NULL;

    } else if (socket == head) {
        head = head->next;
        head->prev = NULL;

    } else if (socket == tail) {
        tail = tail->prev;
        tail->next = NULL;

    } else {
        socket->prev->next = socket->next;
        socket->next->prev = socket->prev;
    }

    free(socket);
}

struct client_sock* get_head() {
    return head;
} 





