#ifndef ONIX_TTY_H
#define ONIX_TTY_H

#define TTY_COMMAND_SIZE 256

extern char command[TTY_COMMAND_SIZE];

void task_tty();
void init_tty();

void key_handler(int key);

#endif