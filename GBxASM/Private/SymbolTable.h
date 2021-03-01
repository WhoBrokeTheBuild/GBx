#ifndef GBXASM_SYMBOL_TABLE_H
#define GBXASM_SYMBOL_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define SYM_MAX_NAME_LEN (32)

typedef struct _Symbol
{
    char Name[SYM_MAX_NAME_LEN];

    long Address;

} Symbol;

typedef struct _SymbolTable
{
    size_t Count;

    size_t Capacity;

    Symbol * Entries;

} SymbolTable;

void SymbolTable_Init(SymbolTable * table);

void SymbolTable_Term(SymbolTable * table);

void SymbolTable_Clear(SymbolTable * table);

bool SymbolTable_Grow(SymbolTable * table, size_t newCapacity);

bool SymbolTable_Add(SymbolTable * table, const char * name, long address);

long SymbolTable_Get(SymbolTable * table, const char * name);

#endif // GBXASM_SYMBOL_TABLE_H