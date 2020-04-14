#ifndef SM83_DEBUG_H
#define SM83_DEBUG_H

#include <SM83/Types.h>

typedef struct sm83 sm83_t;

uint16_t SM83_GetLastInstructionAddress(sm83_t * ctx);

void SM83_EnableInstructionLogging(sm83_t * ctx);

void SM83_AddInstructionLogEntry(sm83_t * ctx, const char * format, ...);

const char * SM83_GetInstructionLogEntry(sm83_t * ctx, int index);

void SM83_EnableStackLogging(sm83_t * ctx);

void SM83_PushStackLogEntry(sm83_t * ctx, const char * format, ...);

void SM83_PopStackLogEntry(sm83_t * ctx);

const char * SM83_GetStackLogEntry(sm83_t * ctx, int index);

#endif // SM83_DEBUG_H