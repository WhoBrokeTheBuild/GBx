#include "SymbolTable.h"

#include <stdio.h>
#include <ctype.h>

#include "Util.h"

#define SYMBOL_TABLE_INITIAL_SIZE (10)

void SymbolTable_Init(SymbolTable * table)
{
    table->Capacity = 0;
    table->Count = 0;
    table->Entries = NULL;

    SymbolTable_Grow(table, SYMBOL_TABLE_INITIAL_SIZE);
}

void SymbolTable_Term(SymbolTable * table)
{
    table->Capacity = 0;
    table->Count = 0;

    free(table->Entries);
    table->Entries = NULL;
}

void SymbolTable_Clear(SymbolTable * table)
{
    for (size_t i = 0; i < table->Capacity; ++i) {
        Symbol * entry = &table->Entries[i];

        entry->Name[0] = '\0';
        entry->Address = LONG_MIN;
    }

    table->Count = 0;
}

bool SymbolTable_Grow(SymbolTable * table, size_t newCapacity)
{
    if (table->Capacity >= newCapacity) {
        return true;
    }

    size_t newSize = newCapacity * sizeof(Symbol);
    void * newEntries = realloc(table->Entries, newSize);

    if (newEntries) {
        table->Entries = (Symbol *)newEntries;

        // Initialize new entries
        for (size_t i = table->Capacity; i < newCapacity; ++i) {
            Symbol * entry = &table->Entries[i];

            entry->Name[0] = '\0';
            entry->Address = LONG_MIN;
        }

        table->Capacity = newCapacity;

        return true;
    }

    return false;
}

bool SymbolTable_Add(SymbolTable * table, const char * name, long address)
{
    if (table->Capacity == table->Count) {
        if (!SymbolTable_Grow(table, table->Capacity * 2)) {
            // Unable to grow
            return false;
        }
    }

    for (size_t i = 0; i < table->Capacity; ++i) {
        Symbol * entry = &table->Entries[i];
        
        if (entry->Name[0] == '\0') {
            strncpy(entry->Name, name, SYM_MAX_NAME_LEN - 1);
            entry->Name[SYM_MAX_NAME_LEN - 1] = '\0';

            entry->Address = address;
            ++table->Count;
            return true;
        }
    }

    // Unable to find open slot, somehow
    return false;
}

long SymbolTable_Get(SymbolTable * table, const char * name)
{
    for (size_t i = 0; i < table->Capacity; ++i) {
        Symbol * entry = &table->Entries[i];

        if (strncmp(entry->Name, name, SYM_MAX_NAME_LEN) == 0) {
            return entry->Address;
        }
    }

    return LONG_MIN;
}
