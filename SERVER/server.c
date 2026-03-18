#include "client.h"

int main() {

    struct listen_sock passive;

    setup_socket(&passive);
    
    for (;;) {

        fd_set fds;
        int lfd = passive.sock_fd;

        FD_ZERO(&fds);
        FD_SET(passive.sock_fd, &fds);

        // Build fd_set
        struct client_sock *curr = get_head();
        while (curr != NULL) {
            int curr_fd = curr->sock_fd;

            if (curr_fd > lfd) {
                lfd = curr_fd;
            }

            FD_SET(curr_fd, &fds);
            curr = curr->next;
        }
        lfd += 1;
        
        // Wait until a socket is ready
        int ret = select(lfd, &fds, NULL, NULL, NULL);

        // Accept new connection if ready
        if (FD_ISSET(passive.sock_fd, &fds)) {
            // accept
        }

        // Read from ready sockets
        curr = get_head();
        while (curr != NULL) {

            if (FD_ISSET(curr->sock_fd), &fds) {
                // read stuff
            }

            curr = curr->next;
        }
    }


}