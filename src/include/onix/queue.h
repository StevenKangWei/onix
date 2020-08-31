#ifndef ONIX_QUEUE_H
#define ONIX_QUEUE_H

#include <onix/type.h>

typedef struct Queue
{
    char *head;
    char *tail;
    int length;
    int count;
    int size;
    void *buffer;
} Queue;

void init_queue(Queue *queue, void *buffer, int length, int size);
void destory_queue(Queue *queue);
bool queue_empty(Queue *queue);
void enqueue(Queue *queue, void *data);
int dequeue(Queue *queue, void *data);
int popqueue(Queue *queue, void *data);
void clear_queue(Queue *queue);
#endif