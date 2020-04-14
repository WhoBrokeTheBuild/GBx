#ifndef GBX_TYPES_DEBUG_H
#define GBX_TYPES_DEBUG_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <GBx/Macros.h>

typedef struct memory_tracker_entry
{
    uint8_t Read:4;
    uint8_t Write:4;

} gbx_memory_tracker_entry_t;

static_assert(sizeof(gbx_memory_tracker_entry_t) == 1, 
    "sizeof(gbx_memory_tracker_entry_t) != 1");

#endif // GBX_TYPES_DEBUG_H