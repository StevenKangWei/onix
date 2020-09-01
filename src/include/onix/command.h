#ifndef ONIX_COMMAND_H
#define ONIX_COMMAND_H

#include <onix/type.h>

void command_beep();
void command_test();
bool execute(const char *command);

#endif