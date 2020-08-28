#ifndef ONIX_STRING_H
#define ONIX_STRING_H
#include "type.h"

size_t strlen(const char *string);
void memcpy(void *dest, void *src, size_t size);
void memset(void *dest, char ch, size_t size);
void strcpy(char *dest, char* src);

#endif