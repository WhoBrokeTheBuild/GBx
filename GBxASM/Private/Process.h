#ifndef GBXASM_PROCESS_H
#define GBXASM_PROCESS_H

#include "Context.h"

void ProcessData(GBxASM * ctx, char * args);

bool ProcessArgument(GBxASM * ctx, char * arg, ArgumentType type, long baseAddress);

bool ProcessInstruction(GBxASM * ctx, char * name, char * args);

bool ProcessFile(GBxASM * ctx, const char * filename);

#endif // GBXASM_PROCESS_H