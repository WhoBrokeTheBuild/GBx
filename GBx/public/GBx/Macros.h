#ifndef GBX_MACROS_H
#define GBX_MACROS_H

#if defined(__MSC_VER)
#    define GBX_PACK(DECL) __pragma(pack(push, 1)) DECL __pragma(pack(pop))
#else
#    define GBX_PACK(DECL) DECL __attribute__((__packed__))
#endif

#endif // GBX_MACROS_H