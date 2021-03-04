#ifndef GBX_LOG_H
#define GBX_LOG_H

#include <GBx/GBx.h>

#include <clog.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GBxLogFatal(M, ...)                                               \
    do {                                                                  \
        clog_log(CLOG_COLOR_RED, "[FATA](%s:%d) " M "\n",                 \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
        exit(1);                                                          \
    } while (0)

#define GBxLogError(M, ...)                                               \
    do {                                                                  \
        clog_log(CLOG_COLOR_RED, "[ERRO](%s:%d) " M "\n",                 \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#define GBxLogWarning(M, ...)                                             \
    do {                                                                  \
        clog_log(CLOG_COLOR_YELLOW, "[WARN](%s:%d) " M "\n",              \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#define GBxLogInfo(M, ...)                                                \
    do {                                                                  \
        clog_log(CLOG_COLOR_DEFAULT, "[INFO](%s:%d) " M "\n",             \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#define GBxLogLoad(M, ...)                                                \
    do {                                                                  \
        clog_log(CLOG_COLOR_GREEN, "[LOAD](%s:%d) " M "\n",               \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#define GBxPerformance(M, ...)                                            \
    do {                                                                  \
        clog_log(CLOG_COLOR_MAGENTA, "[PERF](%s:%d) " M "\n",             \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#define GBxLogVerbose(M, ...)                                             \
    do {                                                                  \
        clog_log(CLOG_COLOR_DEFAULT, "[VERB](%s:%d) " M "\n",             \
            CLOG_FILENAME, __LINE__, ##__VA_ARGS__);                      \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif // GBX_LOG_H