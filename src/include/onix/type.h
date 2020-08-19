#ifndef ONIX_TYPE_H
#define ONIX_TYPE_H

#define _packed __attribute__((packed))

#ifndef _BOOL
typedef unsigned char bool;
static const bool false = 0;
static const bool true = 1;
#endif

typedef unsigned int size_t;
typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int Dword;
typedef unsigned long long Qword;

typedef Byte u8;
typedef Word u16;
typedef Dword u32;
typedef Qword u64;

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long long ulong;

#define INT_MAX 2147483647
#define INT_MIN -2147483648

#include <onix/stdarg.h>

#endif
