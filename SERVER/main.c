#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

#include "socket.h"
#include "client.h"

int sig_int_recieved = 0;

void sigint_handler(int sig) {
    (void)sig;
    sig_int_recieved = 1;
}

int main() {

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, sigint_handler);

    struct listen_sock passive;
    setup_listen_socket(&passive);

    int max_fd = passive.sock_fd;
    fd_set all_fds, ready_fds;

    FD_ZERO(&all_fds);
    FD_SET(passive.sock_fd, &all_fds);

    while (!sig_int_recieved) {

        ready_fds = all_fds;

        // Wait until a socket is ready
        int ret = select(max_fd + 1, &ready_fds, NULL, NULL, NULL);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("server: select");
            continue;
        }

        // Accept new connection if ready
        if (FD_ISSET(passive.sock_fd, &ready_fds)) {
            int client_fd = accept_connection(passive.sock_fd);
            if (client_fd < 0) {
                printf("Error accepting connection\n");
                continue;
            }

            if (client_fd > max_fd) {
                max_fd = client_fd;
            }

            FD_SET(client_fd, &all_fds);
            printf("Accepted connection: %d\n", client_fd);
        }

        // Read from ready sockets
        struct client_sock *curr = get_head();
        while (curr != NULL) {
            struct client_sock *next = get_next(curr);
            int fd = get_sock_fd(curr);

            if (FD_ISSET(fd, &ready_fds)) {
                int result = read_from_client(curr);

                if (result == -1 || result == 1) {
                    FD_CLR(fd, &all_fds);
                    close(fd);
                    remove_client(curr);
                    printf("Client disconnected: %d\n", fd);
                    curr = next;
                    continue;
                }
                
                char *msg;
                while (get_message(&msg, curr) == 0) {
                    printf("%s\n", msg);
                    // handle(msg);
                    free(msg);
                }
            }

            curr = next;
        }
    }

    struct client_sock *curr = get_head();
    while (curr != NULL) {
        struct client_sock *next = get_next(curr);
        close(get_sock_fd(curr));
        remove_client(curr);
        curr = next;
    }

    close(passive.sock_fd);
    free(passive.addr);

    printf("\nServer shut down.\n");
    return 0;
}