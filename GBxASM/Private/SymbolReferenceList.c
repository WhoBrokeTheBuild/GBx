#include "SymbolReferenceList.h"

#define SYMBOL_REFERENCE_LIST_INITIAL_SIZE (10)

void SymbolReferenceList_Init(SymbolReferenceList * list)
{
    list->Capacity = 0;
    list->Count = 0;
    list->Entries = NULL;

    SymbolReferenceList_Grow(list, SYMBOL_REFERENCE_LIST_INITIAL_SIZE);
}

void SymbolReferenceList_Term(SymbolReferenceList * list)
{
    list->Capacity = 0;
    list->Count = 0;

    free(list->Entries);
    list->Entries = NULL;
}

void SymbolReferenceList_Clear(SymbolReferenceList * list)
{
    for (size_t i = 0; i < list->Capacity; ++i) {
        SymbolReference * entry = &list->Entries[i];

        entry->Name[0] = '\0';
        entry->Type = ARG_TYPE_NONE;
        entry->WriteOffset = LONG_MIN;
        entry->BaseAddress = LONG_MIN;
    }

    list->Count = 0;
}

bool SymbolReferenceList_Grow(SymbolReferenceList * list, size_t newCapacity)
{
    if (list->Capacity >= newCapacity) {
        return true;
    }

    size_t newSize = newCapacity * sizeof(SymbolReference);
    void * newEntries = realloc(list->Entries, newSize);

    if (newEntries) {
        list->Entries = (SymbolReference *)newEntries;

        // Initialize new entries
        for (size_t i = list->Capacity; i < newCapacity; ++i) {
            SymbolReference * entry = &list->Entries[i];

            entry->Name[0] = '\0';
            entry->Type = ARG_TYPE_NONE;
            entry->WriteOffset = LONG_MIN;
            entry->BaseAddress = LONG_MIN;
        }

        list->Capacity = newCapacity;

        return true;
    }

    return false;
}

bool SymbolReferenceList_Add(SymbolReferenceList * list, const char * name,
    ArgumentType type, long writeOffset, long baseAddress)
{
    if (writeOffset < 0) {
        return false;
    }

    if (list->Capacity == list->Count) {
        if (!SymbolReferenceList_Grow(list, list->Capacity * 2)) {
            // Unable to grow
            return false;
        }
    }

    for (size_t i = 0; i < list->Capacity; ++i) {
        SymbolReference * entry = &list->Entries[i];

        if (entry->Name[0] == '\0') {
            strncpy(entry->Name, name, SYM_MAX_NAME_LEN - 1);
            entry->Name[SYM_MAX_NAME_LEN - 1] = '\0';

            entry->Type = type;
            entry->WriteOffset = writeOffset;

            if (type == ARG_TYPE_S8 || type == ARG_TYPE_SP_S8) {
                entry->BaseAddress = baseAddress;
            }
            else {
                entry->BaseAddress = 0;
            }

            ++list->Count;
            return true;
        }
    }

    // Unable to find open slot, somehow
    return false;
}

void SymbolReferenceList_Write(SymbolReferenceList * list, 
    SymbolTable * table, FILE * file)
{
    long origin = ftell(file);

    for (size_t i = 0; i < list->Capacity; ++i) {
        SymbolReference * entry = &list->Entries[i];
        
        if (entry->Name[0] != '\0') {
            long value = SymbolTable_Get(table, entry->Name);

            if (value == LONG_MIN) {
                // Error
                continue;
            }

            if (entry->BaseAddress != LONG_MIN) {
                value -= entry->BaseAddress;
            }

            fseek(file, entry->WriteOffset, SEEK_SET);

            if (entry->Type == ARG_TYPE_U8 || 
                entry->Type == ARG_TYPE_ADDR_U8 ||
                entry->Type == ARG_TYPE_FF00_U8) {

                uint8_t data = value;
                fwrite(&data, sizeof(data), 1, file);
            }
            else if (entry->Type == ARG_TYPE_U16 || 
                entry->Type == ARG_TYPE_ADDR_U16 ) {

                uint16_t data = value;
                fwrite(&data, sizeof(data), 1, file);
            }
            else if (entry->Type == ARG_TYPE_S8 ||
                entry->Type == ARG_TYPE_SP_S8) {

                int8_t data = value;
                fwrite(&data, sizeof(data), 1, file);
            }

            // Remove entry
            entry->Name[0] = '\0';
            entry->Type = ARG_TYPE_NONE;
            entry->WriteOffset = LONG_MIN;
            entry->BaseAddress = LONG_MIN;
            --list->Count;
        }
    }

    fseek(file, origin, SEEK_SET);
}

void SymbolReferenceList_PrintUnresolvedSymbols(SymbolReferenceList * list)
{
    for (size_t i = 0; i < list->Capacity; ++i) {
        SymbolReference * entry = &list->Entries[i];
        
        if (entry->Name[0] != '\0') {
            printf("Unresolved Symbol: %s\n", entry->Name);
        }
    }
}
