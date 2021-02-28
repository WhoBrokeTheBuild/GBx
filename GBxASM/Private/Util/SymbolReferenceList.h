#ifndef GBXASM_SYMBOL_REFERENCE_LIST_H
#define GBXASM_SYMBOL_REFERENCE_LIST_H

#include "SymbolTable.h"

#include <stdio.h>

typedef enum _SymbolReferenceType
{
    SYM_REF_TYPE_NONE = 0,
    SYM_REF_TYPE_U8,
    SYM_REF_TYPE_U16,
    SYM_REF_TYPE_S8,

} SymbolReferenceType;

typedef struct _SymbolReference
{
    char Name[SYMBOL_MAX_NAME_LENGTH];

    SymbolReferenceType Type;

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
    SymbolReferenceType type, long writeOffset, long baseAddress);

void SymbolReferenceList_Write(SymbolReferenceList * list, 
    SymbolTable * table, FILE * file);

void SymbolReferenceList_PrintUnresolvedSymbols(SymbolReferenceList * list);

#endif // GBXASM_SYMBOL_REFERENCE_LIST_H