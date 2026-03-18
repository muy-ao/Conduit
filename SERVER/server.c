#include "client.h"

int main() {

    struct listen_sock passive;
    setup_listen_socket(&passive);

    int max_fd = passive.sock_fd;
    fd_set all_fds, ready_fds;

    FD_ZERO(&all_fds);
    FD_SET(passive.sock_fd, &all_fds);

    for (;;) {

        ready_fds = all_fds;

        // Wait until a socket is ready
        int ret = select(max_fd + 1, &ready_fds, NULL, NULL, NULL);

        // Accept new connection if ready
        if (FD_ISSET(passive.sock_fd, &ready_fds)) {
            // accept — then add to all_fds:
            // FD_SET(client_fd, &all_fds);
            // if (client_fd > max_fd) max_fd = client_fd;
        }

        // Read from ready sockets
        struct client_sock *curr = get_head();
        while (curr != NULL) {

            if (FD_ISSET(curr->sock_fd, &ready_fds)) {
                // read stuff
                // if client disconnects:
                // FD_CLR(curr->sock_fd, &all_fds);
            }

            curr = curr->next;
        }
    }


}