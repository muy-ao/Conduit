#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "socket.h"
#include "client.h"

// -1 error
int accept_connection(int fd) {
    struct sockaddr_in peer;
    unsigned int peer_len = sizeof(peer);

    peer.sin_family = AF_INET;
    int client_fd = accept(fd, (struct sockaddr *)&peer, &peer_len);

    if (client_fd < 0) {
        perror("server: accept");
        return -1;
    }

    add_client(client_fd);
    return client_fd;
}   


void setup_listen_socket(struct listen_sock *passive) {

    passive->addr = malloc(sizeof(struct sockaddr_in));
    if (passive->addr == NULL) {
        perror("malloc");
        exit(1);
    }

    // Create socket
    passive->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (passive->sock_fd < 0) {
        perror("server: socket");
        free(passive->addr);
        exit(1);
    }

    memset(&(passive->addr->sin_zero), 0, 8);
    passive->addr->sin_family = AF_INET;
    passive->addr->sin_port = htons(SERVER_PORT);
    passive->addr->sin_addr.s_addr = INADDR_ANY;

    // Socket opt so we can reuse port instantly
    int on = 1;
    int status = setsockopt(passive->sock_fd, SOL_SOCKET, SO_REUSEADDR, 
        (const char *) &on, sizeof(on));
    
    if (status < 0) {
        perror("setsockopt");
        free(passive->addr);
        close(passive->sock_fd);
        exit(1);
    }

    // Bind port to socket
    status = bind(passive->sock_fd, (struct sockaddr *)passive->addr, 
        sizeof(*(passive->addr)));

    if (status < 0) {
        perror("server: bind");
        free(passive->addr);
        close(passive->sock_fd);
        exit(1);
    }

    // Announce willingness to accept connections
    status = listen(passive->sock_fd, MAX_BACKLOG);

    if (status < 0) {
        perror("server: listen");
        free(passive->addr);
        close(passive->sock_fd);
        exit(1);
    }
}

// -1 not found
// index of CRLF otherwise
int find_CRLF(char *buf, int inbuf) {
    for (int i = 0; i < inbuf - 1; i++) {
        if (buf[i] == '\r' && buf[i + 1] == '\n') {
            return i + 2;
        }
    }
    return -1;
}

// -1 error
// 0 terminated msg found
// 1 socket closed
// 2 partial msg
int read_from_client(struct client_sock *client) {

    int *inbuf = &(client->inbuf);
    char *buf = client->buf;

    if (*inbuf == BUF_SIZE) {
        if (find_CRLF(buf, *inbuf) != -1) {
            return 0;
        }
        return -1;
    }

    int count = read(client->sock_fd, buf + *inbuf, BUF_SIZE - *inbuf);
    if (count < 0) {
        return -1;

    } else if (count == 0) {
        return 1;
    }

    *inbuf += count;
    if (find_CRLF(buf, *inbuf) != -1) {
        return 0;
    }
    return 2;
}
