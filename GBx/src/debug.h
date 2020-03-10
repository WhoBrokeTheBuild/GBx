#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>
#include <stdint.h>

extern bool DebugEnable;

void debugInit();
void debugTerm();

void logInstruction(const char * format, ...);
void printInstructionLog();

void debugPrompt();

#endif // DEBUG_H