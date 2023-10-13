#ifndef _PORT_H_
#define _PORT_H_

#include <stdint.h>
#include <stdbool.h>

typedef unsigned char   Uint8;
typedef unsigned int    Uint32;
typedef unsigned short  Uint16;

#define CACHE_LINE  32
#define ptr_t uint32_t
#define CACHE_BIT31 0x80000000

#ifndef NULL
    #define NULL    0
#endif
#define mdelay  sysDelay

#define M_DEBUG sysprintf
#define printk sysprintf

#endif

