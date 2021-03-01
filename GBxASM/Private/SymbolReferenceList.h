#ifndef GBXASM_SYMBOL_REFERENCE_LIST_H
#define GBXASM_SYMBOL_REFERENCE_LIST_H

#include "SymbolTable.h"
#include "Instruction.h"

#include <stdio.h>

typedef struct _SymbolReference
{
    char Name[SYM_MAX_NAME_LEN];

    ArgumentType Type;

    long WriteOffset;

    long BaseAddress;

} SymbolReference;

typedef struct _SymbolReferenceList
{
    size_t Count;

    size_t Capacity;

    SymbolReference * Entries;

} SymbolReferenceList;

void SymbolReferenceList_Init(SymbolReferenceList * list);

void SymbolReferenceList_Term(SymbolReferenceList * list);

void SymbolReferenceList_Clear(SymbolReferenceList * list);

bool SymbolReferenceList_Grow(SymbolReferenceList * list, size_t newCapacity);

bool SymbolReferenceList_Add(SymbolReferenceList * list, const char * name,
    ArgumentType type, long writeOffset, long baseAddress);

void SymbolReferenceList_Write(SymbolReferenceList * list, 
    SymbolTable * table, FILE * file);

void SymbolReferenceList_PrintUnresolvedSymbols(SymbolReferenceList * list);

#endif // GBXASM_SYMBOL_REFERENCE_LIST_H