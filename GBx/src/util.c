#include "util.h"

#include <GBx/log.h>

#include <string.h>

#if defined(WIN32)
#   include <Windows.h>
#else
#   include <unistd.h>
#endif

void GetExecutablePath(char * buffer, size_t length)
{
#if defined(WIN32)

    int newlen = GetModuleFileNameA(NULL, buffer, length - 1);
    if (newlen == 0) {
        LogError("GetModuleFileNameA failed");
        newlen = 0;
    }
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        LogError("Path too long");
    }

    char * lastSlash = strrchr(buffer, '\\');
    if (!lastSlash) {
        lastSlash = strrchr(buffer, '/');
    }
    if (lastSlash) {
        *(lastSlash + 1) = '\0';
    }

    buffer[newlen + 1] = '\0';

#else

    int newlen = readlink("/proc/self/exe", buffer, length - 1);
    if (newlen < 0) {
        LogError("Failed to resolve symlink to /proc/self/exe");
        newlen = 0;
    }
    else if (newlen >= length) {
        LogError("Path too long");
    }

    char * lastSlash = strrchr(buffer, '/');
    if (lastSlash) {
        *(lastSlash + 1) = '\0';
    }

    buffer[newlen + 1] = '\0';

#endif
}