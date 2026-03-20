#include "queue.h"


void handle(char *msg) {
    
    struct group *group = 
}


void add_single_to_queue

void add_group_to_queue

// if message recieved from lobby server: modify queue
// if queue is full, ask game manager for server.

// if message recieved from game manager, it must be that a server
// is ready. send message to lobby to send all players to server

// if message recieved from a server, it must be that the game
// finished or the server terminated. send all related players 
// back to lobby.




