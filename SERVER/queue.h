#ifndef _QUEUE_H_
#define QUEUE_H_

#define MAX_QUEUE_SIZE 2

struct group {
    char uuids[MAX_QUEUE_SIZE][37] = {0};
    int num_uuids;
};

#endif