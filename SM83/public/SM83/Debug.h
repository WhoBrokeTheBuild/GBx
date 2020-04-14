#ifndef SM83_DEBUG_H
#define SM83_DEBUG_H

#include <SM83/Types.h>

typedef struct sm83 sm83_t;

uint16_t SM83_GetLastInstructionAddress(sm83_t * ctx);

uint16_t SM83_GetStackBaseAddress(sm83_t * ctx);

void SM83_EnableInstructionLog();

void SM83_AddInstructionLogEntry(sm83_t * ctx, const char * inst);

const char * SM83_GetInstructionLogEntry(sm83_t * ctx, int index);

void SM83_PushStackLogEntry(sm83_t * ctx, const char * inst);

void SM83_PopStackLogEntry(sm83_t * ctx);

#endif // SM83_DEBUG_H