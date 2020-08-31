#ifndef ONIX_TTY_H
#define ONIX_TTY_H

#include <onix/queue.h>

#define TTY_COMMAND_SIZE 256

extern Queue command_queue;
extern char buffer[TTY_COMMAND_SIZE];

void task_tty();
void init_tty();
void put_key(char key);

void key_handler(int key);

void print_prompt();

#endif