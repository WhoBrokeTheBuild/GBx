#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdbool.h>

void SetBreakpoint(const char * reg, unsigned value);

void ClearBreakpoint(const char * reg, unsigned value);

void ClearAllBreakpoints();

bool AtBreakpoint();

void PrintBreakpoints();

#endif // BREAKPOINT_H