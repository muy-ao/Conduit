

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