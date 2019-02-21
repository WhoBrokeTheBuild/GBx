#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <stdbool.h>

extern bool DebugMode;

typedef struct {
    unsigned int PC;
} breakpoint_t;

extern breakpoint_t B;

void debugPrompt();

#endif // DEBUG_H