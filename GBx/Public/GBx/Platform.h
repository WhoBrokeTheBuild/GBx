#ifndef GBX_PLATFORM_H
#define GBX_PLATFORM_H

#if defined(_WIN32)

    // Windows
    #define GBX_PLATFORM_WINDOWS

#elif defined(__APPLE__) && defined(__MACH__)

    // OSX or iOS
    #define GBX_PLATFORM_APPLE

#elif defined(__unix__)

    #if defined(__linux__)
        // Linux
        #define GBX_PLATFORM_LINUX
    #else
        #error Unsupported UNIX Operating System
    #endif

#else

    // Unknown
    #error Unsupported Operating System

#endif

#if defined(_MSC_VER)

    // Microsoft VisualStudio C/C++
    #define GBX_COMPILER_MSVC _MSC_VER

#elif defined(__clang__)

    // Apple Clang Compiler
    #define GBX_COMPILER_CLANG __clang__

#elif defined(__GNUC__)
    
    // GNU Compiler Collection
    #define GBX_COMPILER_GCC __GNUC__

#else

    // Unknown
    #warning Unsupported Compiler

#endif

#if defined(NDEBUG)

    #define GBX_BUILD_RELEASE

#else

    #define GBX_BUILD_DEBUG

#endif

#endif // GBX_PLATFORM_H