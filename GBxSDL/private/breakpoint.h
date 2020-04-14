#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdbool.h>

#include <GBx/GBx.h>

void SetBreakpoint(const char * reg, unsigned value);

void SetBreakpointStackChanged(gbx_t * ctx);

void ClearBreakpoint(const char * reg, unsigned value);

void ClearAllBreakpoints();

bool AtBreakpoint(gbx_t * ctx);

void PrintBreakpoints();

#endif // BREAKPOINT_H