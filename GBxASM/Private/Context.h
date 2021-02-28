#ifndef GBXASM_CONTEXT_H
#define GBXASM_CONTEXT_H

#include "Util/SymbolTable.h"
#include "Util/SymbolReferenceList.h"

#include <stdio.h>

typedef struct _GBxASM
{
    FILE * Output;

    SymbolTable GlobalSymbols;

    SymbolTable LocalSymbols;

    SymbolReferenceList References;

} GBxASM;

static inline long GetWriteAddress(GBxASM * ctx)
{
    return ftell(ctx->Output);
}

static inline void WriteU8(GBxASM * ctx, uint8_t data)
{
    fwrite(&data, sizeof(uint8_t), 1, ctx->Output);
    fflush(ctx->Output);
}

static inline void WriteS8(GBxASM * ctx, int8_t data)
{
    fwrite(&data, sizeof(int8_t), 1, ctx->Output);
    fflush(ctx->Output);
}

static inline void WriteU16(GBxASM * ctx, uint16_t data)
{
    fwrite(&data, sizeof(uint16_t), 1, ctx->Output);
    fflush(ctx->Output);
}

#endif // GBXASM_CONTEXT_H