#ifndef SM83_DEBUG_H
#define SM83_DEBUG_H

#include <SM83/Context.h>
#include <SM83/Types.h>

void SM83_AddInstructionLogEntry(sm83_t * cpu, const char * inst);

const char * SM83_GetInstructionLogEntry(sm83_t * cpu, int index);

#endif // SM83_DEBUG_H