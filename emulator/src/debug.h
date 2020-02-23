#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <stdint.h>

extern bool DebugEnable;

void debugInit();
void debugTerm();

void setBreakpoint(uint16_t address);
void clearBreakpoint();
bool atBreakpoint();

void logInstruction(const char * format, ...);
void printInstructionLog();

void debugPrompt();

#endif // DEBUG_H