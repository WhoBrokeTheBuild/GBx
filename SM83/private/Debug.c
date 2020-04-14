#include <SM83/Context.h>
#include <SM83/Debug.h>

#include "Internal.h"

#include <string.h>

uint16_t SM83_GetLastInstructionAddress(sm83_t * ctx)
{
    return ctx->internal->LastInstructionAddress;
}

uint16_t SM83_GetStackBaseAddress(sm83_t * ctx)
{
    return ctx->internal->StackBaseAddress;
}

void SM83_AddInstructionLogEntry(sm83_t * ctx, const char * inst)
{
    sm83_internal_t * inctx = ctx->internal;

    if (!inctx->InstructionLoggingEnabled) {
        return;
    }

    strncpy(inctx->InstructionLog[inctx->InstructionLogIndex],
        inst,
        INSTRUCTION_LOG_ENTRY_SIZE);

    ++inctx->InstructionLogIndex;
    inctx->InstructionLogIndex %= INSTRUCTION_LOG_LENGTH;
}

const char * SM83_GetInstructionLogEntry(sm83_t * ctx, int index)
{
    sm83_internal_t * inctx = ctx->internal;

    if (!inctx->InstructionLoggingEnabled) {
        return NULL;
    }

    index = inctx->InstructionLogIndex - index - 1;
    if (index < 0) {
        index += INSTRUCTION_LOG_LENGTH;
    }

    return inctx->InstructionLog[index];
}