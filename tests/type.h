#ifndef ONIX_TYPE_H
#define ONIX_TYPE_H

#define PACKED __attribute__((packed))

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;

#ifndef _BOOL
typedef unsigned char bool;
static const bool false = 0;
static const bool true = 1;
#endif

#endif