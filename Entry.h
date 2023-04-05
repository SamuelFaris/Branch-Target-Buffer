
#ifndef _BTB_ENTRY_H
#define _BTB_ENTRY_H

#include <stdint.h>

struct Entry
{
    uint32_t pc;
    uint32_t target;
    uint32_t prediction;
};

#endif