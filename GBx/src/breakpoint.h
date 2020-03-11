#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdbool.h>

void setBreakpoint(const char * reg, unsigned value);

void clearBreakpoint(const char * reg, unsigned value);

void clearAllBreakpoints();

void printBreakpoints();

bool atBreakpoint();

#endif // BREAKPOINT_H