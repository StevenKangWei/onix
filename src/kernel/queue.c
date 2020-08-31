#include <onix/queue.h>
#include <onix/string.h>
#include <onix/stdio.h>

void init_queue(Queue *queue, void *buffer, int length, int size)
{
    queue->length = length;
    queue->size = size;
    queue->count = 0;
    queue->head = queue->tail = buffer;
    queue->buffer = buffer;
    memset(buffer, 0, length);
}

void destory_queue(Queue *queue)
{
}

bool queue_empty(Queue *queue)
{
    return (queue->count == 0);
}

void enqueue(Queue *queue, void *data)
{
    memcpy(queue->head, data, queue->size);

    queue->head += queue->size;
    queue->count++;

    if (queue->head >= queue->buffer + queue->length)
    {
        queue->head = queue->buffer;
    }
    if (queue->head == queue->tail)
    {
        queue->tail += queue->size;
        queue->count--;
    }
    if (queue->tail >= queue->buffer + queue->length)
    {
        queue->tail = queue->buffer;
    }
}

int dequeue(Queue *queue, void *data)
{
    if (queue->count <= 0)
        return 0;

    memcpy(data, queue->tail, queue->size);
    queue->tail += queue->size;
    queue->count--;
    if (queue->tail >= queue->buffer + queue->length)
    {
        queue->tail = queue->buffer;
    }
    return 1;
}

int popqueue(Queue *queue, void *data)
{
    if (queue->count <= 0)
        return 0;

    memcpy(data, (queue->head - queue->size), queue->size);
    queue->head -= queue->size;
    queue->count--;

    if (queue->head < queue->buffer)
    {
        queue->head = queue->buffer + queue->length - queue->size;
    }
    return 1;
}

void clear_queue(Queue *queue)
{
    queue->count = 0;
    queue->head = queue->tail = queue->buffer;
    memset(queue->buffer, 0, queue->length);
}