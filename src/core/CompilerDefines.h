#pragma once
#ifndef HD_INC_OSLAYER_COMPILER_DEFINES_H
#define HD_INC_OSLAYER_COMPILER_DEFINES_H

/**
---- To check for which OS the code is compiled:
Linux and Linux-derived           __linux__
Android                           __ANDROID__ (implies __linux__)
Linux (non-Android)               __linux__ && !__ANDROID__
Darwin (Mac OS X and iOS)         __APPLE__
Akaros (http://akaros.org)        __ros__
windows                           _WIN32
windows 64 bit                    _WIN64 (implies _WIN32)
NaCL                              __native_client__
AsmJS                             __asmjs__
Fuschia                           __Fuchsia__

---- To check which compiler is used:
Visual Studio       _MSC_VER
gcc                 __GNUC__
clang               __clang__
emscripten          __EMSCRIPTEN__ (for asm.js and webassembly)
MinGW 32            __MINGW32__
MinGW-w64 32bit     __MINGW32__
MinGW-w64 64bit     __MINGW64__

---- To check which compiler version is used:
__GNUC__ (e.g. 5) and __GNUC_MINOR__ (e.g. 1).
__clang_major__, __clang_minor__, __clang_patchlevel__
_MSC_VER and _MSC_FULL_VER
 __MINGW64_VERSION_MAJOR and __MINGW64_VERSION_MINOR

---- To check which compiler architecture is used:
msvc:
_WIN32 // 32-bit ARM, 64 bits ARM, x86 ou x64
_WIN64
Gcc :
__i386__ // x86
__x86_64__ // x64
__arm__. If defined, you can further check:
    *   __ARM_ARCH_5T__
    *   __ARM_ARCH_7A__
__powerpc64__
__aarch64__

*/

/** Detect target OS */
#if defined(_WIN32)
    #define HD_OS_WINDOWS
#elif defined(__linux__)
    #define HD_OS_LINUX
#else
    #error Unknown target OS defines
#endif

/** Detect compiler */
#if defined(_MSC_VER) && !defined(__clang__)
#define HD_COMPILER_MSVC
#endif

#if defined(__clang__)
    #if defined(_MSC_VER)
        #define HD_COMPILER_CLANG_CL
    #else
            #define HD_COMPILER_CLANG
    #endif
#endif

/** Detect compiler version */
#if defined(_MSC_VER)
    #define HD_COMPILER_VERSION _MSC_VER
#elif defined(HD_COMPILER_CLANG)
    #define HD_COMPILER_VERSION __clang_major__, __clang_minor__, __clang_patchlevel__
#else
    #error Unknown compiler version defines
#endif

/** Detect target architecture */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    #if defined(_M_X64)
        #define HD_TARGET_X64
    #elif defined (_M_IX86)
     #define HD_TARGET_X86
    #elif defined(_M_ARM64)
        #define HD_TARGET_ARM64
    #elif defined(_M_ARM)
        #define HD_TARGET_ARM32
    #endif
#elif defined(HD_COMPILER_CLANG)
    #if defined(__i386__)
        #define HD_TARGET_X86
    #elif defined(__x86_64__)
        #define HD_TARGET_X64
    #endif
#else
    #error Unknown target architecture defines
#endif

#if defined(HD_TARGET_X64) || defined(HD_TARGET_ARM64)
    #define HD_TARGET_64_BITS
#elif defined(HD_TARGET_X86) || defined(HD_TARGET_ARM32)
    #define HD_TARGET_32_BITS
#else
    #error Unknown target architecture defines
#endif

#if defined(HD_TARGET_X64) || defined(HD_TARGET_X86)
#define HD_TARGET_X86_FAMILY
#elif defined(HD_TARGET_ARM64) || defined(HD_TARGET_ARM32)
#define HD_TARGET_ARM_FAMILY
#endif

/** Detect endianness */
#if defined(HD_OS_WINDOWS)
    #define HD_LITTLE_ENDIAN
#elif defined(HD_OS_LINUX)
    #if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define HD_LITTLE_ENDIAN
    #elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define HD_BIG_ENDIAN
    #else
        #error Unknown endianness defines
    #endif
#else
    #error Unknown target OS for endianness defines
#endif



/** Check required defines */
#if !defined( HD_TARGET_32_BITS ) && !defined(HD_TARGET_64_BITS)
#error Architecture targeted not supported
#endif
#if !defined( HD_COMPILER_MSVC ) && !defined(HD_COMPILER_CLANG_CL) && !defined(HD_COMPILER_CLANG)
#error Compiler not supported
#endif
#if !defined( HD_OS_WINDOWS ) && !defined(HD_OS_LINUX)
#error Operating System targeted not supported
#endif
#if !defined(HD_DEBUG) && !defined(HD_DEBUGOPTIMIZED) && !defined(HD_RELEASE)
#error Compilation mode is not set
#endif
#if defined(HD_COMPILER_MSVC) && !(__cplusplus >= 202004L)
#error Compilation should be done with latest C++ version
#endif

#if defined(HD_TARGET_32_BITS)
static_assert(sizeof(void*) == 4, "HD_TARGET_32_BITS is defined but size of pointers are not 4 bytes");
#elif defined(HD_TARGET_64_BITS)
static_assert(sizeof(void*) == 8, "HD_TARGET_64_BITS is defined but size of pointers are not 8 bytes");
#endif

#endif // HD_INC_OSLAYER_COMPILER_DEFINES_H