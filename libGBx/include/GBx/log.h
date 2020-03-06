#ifndef LOG_H
#define LOG_H

#include "cpu.h"
#include "cpu.h"

#include <signal.h>
#include <stdio.h>

extern int VerboseLevel;

#if defined(WIN32)

    #include <Windows.h>

    #define COLOR_CYAN      3
    #define COLOR_RED       4
    #define COLOR_YELLOW    6
    #define COLOR_DEFAULT   7

    #define LogInfo(M, ...)                                         \
        do {                                                        \
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);      \
            SetConsoleTextAttribute(hConsole, COLOR_CYAN);          \
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
            raise(SIGINT);                                          \
        } while (0)

    #define LogVerbose(LVL, M, ...)                             \
        if (VerboseLevel >= LVL) {                              \
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  \
            SetConsoleTextAttribute(hConsole, COLOR_CYAN);      \
            printf("[VERB] " M "\n", ##__VA_ARGS__);            \
            SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);   \
        }

#else // !WIN32

    #define COLOR_RED       "\033[0;31m"
    #define COLOR_YELLOW    "\033[0;33m"
    #define COLOR_CYAN      "\033[0;36m"
    #define COLOR_END       "\033[0;m"

    #define LogInfo(M, ...) \
        do { printf(COLOR_CYAN "[INFO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)
    
    #define LogWarn(M, ...) \
        do { fprintf(stderr, COLOR_YELLOW "[WARN] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)
    
    #define LogError(M, ...) \
        do { fprintf(stderr, COLOR_RED "[ERRO] " M "\n" COLOR_END, ##__VA_ARGS__); } while (0)
    
    #define LogFatal(M, ...) \
        do { fprintf(stderr, COLOR_RED "[FATA] " M "\n" COLOR_END, ##__VA_ARGS__); raise(SIGINT); } while (0)
    
    #define LogVerbose(LVL, M, ...) \
        if (VerboseLevel >= LVL) { printf(COLOR_CYAN "[VERB] " M "\n" COLOR_END, ##__VA_ARGS__); }

#endif // WIN32

#define LogInst(M, ...) \
    if (VerboseLevel >= 3) { printf("[INST] %04X: " M "\n", LastInstructionAddress, ##__VA_ARGS__); }

#endif // LOG_H