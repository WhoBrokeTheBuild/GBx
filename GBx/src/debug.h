#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <stdint.h>

extern bool DebugEnabled;

void DebugInit();
void DebugTerm();

void DebugPrompt();

#endif // DEBUG_H