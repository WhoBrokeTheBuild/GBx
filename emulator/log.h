#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LogInfo(M, ...) printf("[INFO] " M "\n", ##__VA_ARGS__)

#define LogWarn(M, ...) fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__)

#define LogError(M, ...) fprintf(stderr, "[ERRO] " M "\n", ##__VA_ARGS__)

#if defined(NDEBUG)
#   define LogDebug(M, ...)
#else
#   define LogDebug(M, ...) printf("[DEBG] " M "\n", ##__VA_ARGS__)
#endif

#if !defined(VERBOSE)
#   define LogVerbose(M, ...)
#else
#   define LogVerbose(M, ...) printf("[VERB] " M "\n", ##__VA_ARGS__)
#endif

#endif // LOG_H