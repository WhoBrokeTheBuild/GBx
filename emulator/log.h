#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LogInfo(M, ...) \
    do { printf("[INFO] " M "\n", ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { fprintf(stderr, "[ERRO] " M "\n", ##__VA_ARGS__); } while (0)

#define LogFatal(M, ...) \
    do { fprintf(stderr, "[FATA] " M "\n", ##__VA_ARGS__); exit(1); } while (0)

#if defined(NDEBUG)
#   define LogDebug(M, ...) do { } while (0)
#else
#   define LogDebug(M, ...) do { printf("[DEBG] " M "\n", ##__VA_ARGS__); } while (0)
#endif

#if !defined(VERBOSE)
#   define LogVerbose(M, ...)
#else
#   define LogVerbose(M, ...) printf("[VERB] " M "\n", ##__VA_ARGS__)
#endif

#endif // LOG_H