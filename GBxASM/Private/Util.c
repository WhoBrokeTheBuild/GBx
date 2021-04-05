#include "Util.h"

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

char * SkipWhitespace(char * str)
{
    if (!str) {
        return NULL;
    }

    while (isspace(*str)) {
        ++str;
    }

    return str;
}

char * RemoveWhitespace(char * str)
{
    if (!str) {
        return NULL;
    }

    size_t length = strlen(str);

    for (size_t i = 0; i < length; ++i) {
        if (isspace(str[i])) {
            for (size_t j = i; j < length; ++j) {
                str[j] = str[j + 1];
            }
            --length;
            --i;
        }
    }

    return str;
}

bool ParseNumber(char * str, long * num)
{
    *num = LONG_MIN;

    str = SkipWhitespace(str);

    size_t length = (str ? strlen(str) : 0);

    if (length < 1) {
        return false;
    }

    char * endptr = NULL;

    if (str[0] == '$') {
        *num = strtol(str + 1, &endptr, 16);
    }
    else if (str[0] == '0') {
        *num = strtol(str + 1, &endptr, 8);
    }
    else if (str[0] == '-' || isdigit(str[0])) {
        *num = strtol(str, &endptr, 10);
    }

    return (endptr != NULL);
}

char * GetParentPath(const char * filename)
{
    char * pivot = strrchr(filename, '/');
    return (
        pivot
        ? strndup(filename, pivot - filename)
        : NULL
    );
}

bool IsReservedSymbol(const char * name)
{
    const char * RESERVED_SYMBOLS[] = {
        "A",
        "B",
        "C",
        "D",
        "E",
        "H",
        "L",
        "AF",
        "BC",
        "DE",
        "HL",
        "SP",
        "Z",
        "NZ",
        // C
        "NC",
        NULL
    };

    if (!name) {
        return false;
    }

    for (int i = 0; RESERVED_SYMBOLS[i]; ++i) {
        if (strcasecmp(name, RESERVED_SYMBOLS[i]) == 0) {
            return true;
        }
    }

    return false;
}

bool IsValidSymbol(const char * name)
{
    if (IsReservedSymbol(name)) {
        return false;
    }

    size_t length = (name ? strlen(name) : 0);
    if (length == 0) {
        return false;
    }

    // Local Symbol
    if (name[0] == '.') {
        ++name;
    }

    if (isdigit(name[0])) {
        return false;
    }

    // A-Z0-9_, doesn't start with a number
    for (size_t i = 0; i < length; ++i) {
        if (!isalnum(name[i]) && name[i] != '_') {
            return false;
        }
    }

    return true;
}