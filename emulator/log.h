#ifndef LOG_H
#define LOG_H

#include "debug.h"
#include <stdio.h>

#if !defined(WIN32)
#define COLOR_RED       "\033[0;31m"
#define COLOR_BOLD_RED  "\033[1;31m"
#define COLOR_GREEN     "\033[0;32m"
#define COLOR_YELLOW    "\033[0;33m"
#define COLOR_BLUE      "\033[0;34m"
#define COLOR_MAGENTA   "\033[0;35m"
#define COLOR_CYAN      "\033[0;36m"
#define COLOR_END       "\033[0;m"
#endif

#define LogInfo(M, ...) \
    do { printf(COLOR_GREEN "[INFO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { fprintf(stderr, COLOR_YELLOW "[WARN] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { fprintf(stderr, COLOR_RED "[ERRO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogFatal(M, ...) \
    do { fprintf(stderr, COLOR_RED "[FATA] " M "\n" COLOR_END, ##__VA_ARGS__); exit(1); } while (0)

#define LogDebug(M, ...) \
    do { if (DebugMode) { printf(COLOR_CYAN "[DEBG] " M "\n" COLOR_END, ##__VA_ARGS__); } } while (0)

#if !defined(LOG_VERBOSE)
#   define LogVerbose(M, ...)
#else
#   define LogVerbose(M, ...) printf(COLOR_MAGENTA "[VERB] " M "\n" COLOR_END, ##__VA_ARGS__)
#endif

#endif // LOG_H