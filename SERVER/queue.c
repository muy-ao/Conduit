#include "queue.h"

struct group **queue;
int size = 0;
int capacity = 256;

int parent(int i) {
    return (i - 1) / 2;
}

int left_child(int i) {
    return 2 * i + 1;
}

int right_child(int i) {
    return 2 * i + 2;
}

void swap(struct group **a, struct group **b) {
    struct group *temp = *a;
    *a = *b;
    *b = temp;
}

void shift_up(int i) {
    while (i > 0 && queue[parent(i)]->num_uuids < queue[i]->num_uuids) {
        swap(&queue[parent(i)], &queue[i]);
        i = parent(i);
    }
}

void shift_down(int i) {
    int largest = i;
    int l = left_child(i);
    int r = right_child(i);

    if (l < size && queue[l]->num_uuids > queue[largest]->num_uuids) {
        largest = l;
    }
        
    if (r < size && queue[r]->num_uuids > queue[largest]->num_uuids) {
        largest = r;
    }

    if (largest != i) {
        swap(&queue[i], &queue[largest]);
        shift_down(largest);
    }
}

void insert(struct group* group) {
    if (size == capacity) {
        capacity *= 2;
        struct group **temp = realloc(queue, sizeof(struct group*) * capacity);
        if (temp == NULL) {
            perror("realloc");
            exit(1);
        }
        queue = temp;
    }
    queue[size] = group;
    shift_up(size);
    size += 1;
}

void extract_max() {
    queue[0] = queue[size - 1];
    size -= 1;
    shift_down(0);
}

struct group* peek() {
    return queue[0];
}

void setup_queue() {
    struct group **q = malloc(sizeof(struct group*) * 256);
    if (q == NULL) {
        perror("malloc");
        exit(1);
    }
    queue = q;
}