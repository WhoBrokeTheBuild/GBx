#include <SM83/Context.h>
#include <SM83/Debug.h>

#include "Internal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

uint16_t SM83_GetLastInstructionAddress(sm83_t * ctx)
{
    return ctx->internal->LastInstructionAddress;
}

void SM83_EnableInstructionLogging(sm83_t * ctx)
{
    ctx->internal->InstructionLoggingEnabled = true;
}

void SM83_AddInstructionLogEntry(sm83_t * ctx, const char * format, ...)
{
    sm83_internal_t * inctx = ctx->internal;

    if (!inctx->InstructionLoggingEnabled) {
        return;
    }

    va_list args;
    va_start(args, format);

    vsnprintf(inctx->InstructionLog[inctx->InstructionLogIndex], 
        INSTRUCTION_LOG_ENTRY_SIZE,
        format, args);

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

void SM83_EnableStackLogging(sm83_t * ctx)
{
    ctx->internal->StackLoggingEnabled = true;
}

void SM83_PushStackLogEntry(sm83_t * ctx, const char * format, ...)
{
    sm83_internal_t * inctx = ctx->internal;

    if (!inctx->StackLoggingEnabled) {
        return;
    }

    size_t offset = sprintf(inctx->StackLog[inctx->StackLogIndex], 
        "$%04X: ", inctx->LastInstructionAddress);
    
    va_list args;
    va_start(args, format);

    vsnprintf(inctx->StackLog[inctx->StackLogIndex] + offset, 
        STACK_LOG_ENTRY_SIZE - offset,
        format, args);

    ++inctx->StackLogIndex;
}

void SM83_PopStackLogEntry(sm83_t * ctx)
{
    sm83_internal_t * inctx = ctx->internal;
    
    if (!inctx->StackLoggingEnabled) {
        return;
    }
    
    inctx->StackLog[inctx->StackLogIndex][0] = '\0';
    --inctx->StackLogIndex;
}

const char * SM83_GetStackLogEntry(sm83_t * ctx, int index)
{
    sm83_internal_t * inctx = ctx->internal;

    if (!inctx->StackLoggingEnabled) {
        return NULL;
    }

    if (inctx->StackLogIndex <= index) {
        return NULL;
    }

    return inctx->StackLog[inctx->StackLogIndex - index - 1];
}
