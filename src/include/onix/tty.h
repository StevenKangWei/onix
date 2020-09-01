#ifndef ONIX_TTY_H
#define ONIX_TTY_H

#include <onix/queue.h>

#define TTY_COMMAND_SIZE 256

typedef struct TTY{
    Queue queue;
    char buffer[TTY_COMMAND_SIZE];;
} TTY;

extern TTY tty;

void task_tty();
void init_tty();
void put_key(char key);

void key_handler(int key);

void print_prompt();

#endif