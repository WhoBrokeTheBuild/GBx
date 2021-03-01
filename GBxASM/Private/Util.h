#ifndef GBXASM_UTIL_H
#define GBXASM_UTIL_H

#include <string.h>
#include <stdbool.h>

#if defined(_MSC_VER)
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#else
    #include <strings.h>
#endif

char * SkipWhitespace(char * str);

char * RemoveWhitespace(char * str);

bool ParseNumber(char * str, long * num);

char * GetParentPath(const char * filename);

bool IsReservedSymbol(const char * name);

bool IsValidSymbol(const char * name);

#endif // GBXASM_UTIL_H