#ifndef SM83_MACROS_H
#define SM83_MACROS_H

#if defined(__MSC_VER)
#    define SM83_PACK(DECL) __pragma(pack(push, 1)) DECL __pragma(pack(pop))
#    define SM83_INLINE __forceinline
#else
#    define SM83_PACK(DECL) DECL __attribute__((__packed__))
#    define SM83_INLINE __attribute__((always_inline)) 
#endif

#endif // SM83_MACROS_H