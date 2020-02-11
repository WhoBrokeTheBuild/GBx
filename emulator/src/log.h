#ifndef LOG_H
#define LOG_H

#include "debug.h"
#include <stdio.h>

#if defined(WIN32)

#include <Windows.h>

#define COLOR_GREEN     2
#define COLOR_CYAN      3
#define COLOR_RED       4
#define COLOR_MAGENTA   5
#define COLOR_YELLOW    6
#define COLOR_DEFAULT   7

#define LogInfo(M, ...)                                         \
    do {                                                        \
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
        SetConsoleTextAttribute(hConsole, COLOR_GREEN);         \
        printf("[INFO] " M "\n", ##__VA_ARGS__);                \
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);       \
    } while (0)

#define LogWarn(M, ...)                                         \
    do {                                                        \
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
        SetConsoleTextAttribute(hConsole, COLOR_YELLOW);        \
        fprintf(stderr, "[WARN] " M "\n", ##__VA_ARGS__);       \
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);       \
    } while (0)

#define LogError(M, ...)                                        \
    do {                                                        \
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
        SetConsoleTextAttribute(hConsole, COLOR_RED);           \
        fprintf(stderr, "[ERRO] " M "\n", ##__VA_ARGS__);       \
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);       \
    } while (0)

#define LogFatal(M, ...)                                        \
    do {                                                        \
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
        SetConsoleTextAttribute(hConsole, COLOR_RED);           \
        fprintf(stderr, "[FATA] " M "\n", ##__VA_ARGS__);       \
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);       \
    } while (0)

#define LogDebug(M, ...)                                        \
    if (DebugMode) {                                            \
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
        SetConsoleTextAttribute(hConsole, COLOR_CYAN);          \
        printf("[DEBG] " M "\n", ##__VA_ARGS__);                \
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);       \
    }

#if !defined(VERBOSE)
#   define LogVerbose(M, ...) do { } while (0)
#else
#   define LogVerbose(M, ...)                                   \
        do {                                                    \
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  \
            SetConsoleTextAttribute(hConsole, COLOR_MAGENTA);   \
            printf("[VERB] " M "\n", ##__VA_ARGS__);            \
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);   \
        } while (0)
#endif

#else

#define COLOR_RED       "\033[0;31m"
#define COLOR_GREEN     "\033[0;32m"
#define COLOR_YELLOW    "\033[0;33m"
#define COLOR_MAGENTA   "\033[0;35m"
#define COLOR_CYAN      "\033[0;36m"
#define COLOR_END       "\033[0;m"

#define LogInfo(M, ...) \
    do { printf(COLOR_GREEN "[INFO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogWarn(M, ...) \
    do { fprintf(stderr, COLOR_YELLOW "[WARN] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogError(M, ...) \
    do { fprintf(stderr, COLOR_RED "[ERRO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)

#define LogFatal(M, ...) \
    do { fprintf(stderr, COLOR_RED "[FATA] " M "\n" COLOR_END, ##__VA_ARGS__); requestBreakpoint(); } while (0)

#define LogDebug(M, ...) \
    do { if (DebugMode) { printf(COLOR_CYAN "[DEBG] " M "\n" COLOR_END, ##__VA_ARGS__); } } while (0)

#if !defined(VERBOSE)
#   define LogVerbose(M, ...) do { } while (0)
#else
#   define LogVerbose(M, ...) do { printf(COLOR_MAGENTA "[VERB] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)
#endif

#endif // WINDOWS

#endif // LOG_H