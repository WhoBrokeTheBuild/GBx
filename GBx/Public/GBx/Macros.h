#ifndef GBX_MACROS_H
#define GBX_MACROS_H

#if defined(__MSC_VER)
#   define GBX_PACK(DECL) __pragma(pack(push, 1)) DECL __pragma(pack(pop))
#   define GBX_INLINE __forceinline
#   define GBX_RESTRICT __restrict
#else
#   define GBX_PACK(DECL) DECL __attribute__((__packed__))
#   define GBX_INLINE __attribute__((always_inline)) 
#   define GBX_RESTRICT __restrict__
#endif

#if !defined(GBX_SOURCE_PATH_LENGTH)
    #define GBX_SOURCE_PATH_LENGTH 0
#endif

#define GBX_FILENAME (&__FILE__[GBX_SOURCE_PATH_LENGTH])

#define GBxVerbose(CTX, LVL, EXPR)                                             \
    if (GBx_GetVerboseLevel((CTX)) >= (LVL)) {                                 \
        EXPR;                                                                  \
    }

#define GBxOnce(EXPR)                                                          \
    do {                                                                       \
        static bool _Once##__COUNTER__ = false;                                \
        if (!_Once##__COUNTER__) {                                             \
            EXPR;                                                              \
            _Once##__COUNTER__ = true;                                         \
        }                                                                      \
    } while (0)

#define GBxEvery(COUNT, EXPR)                                                     \
    do {                                                                       \
        static int _Every##__COUNTER__ = 0;                                    \
        if (++_Every##__COUNTER__ >= COUNT) {                                  \
            EXPR;                                                              \
            _Every##__COUNTER__ = 0;                                           \
        }                                                                      \
    } while (0)

#endif // GBX_MACROS_H