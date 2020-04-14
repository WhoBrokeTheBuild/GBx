#ifndef GBX_MACROS_H
#define GBX_MACROS_H

#if defined(__MSC_VER)
#    define GBX_PACK(DECL) __pragma(pack(push, 1)) DECL __pragma(pack(pop))
#    define GBX_INLINE __forceinline
#else
#    define GBX_PACK(DECL) DECL __attribute__((__packed__))
#    define GBX_INLINE __attribute__((always_inline)) 
#endif

#define GBx_GetEnabledString(test) ((test) ? "Enabled" : "Disabled")

#endif // GBX_MACROS_H