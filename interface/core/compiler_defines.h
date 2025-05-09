#ifndef HD_INC_CORE_COMPILER_DEFINES_H
#define HD_INC_CORE_COMPILER_DEFINES_H

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
Emscripten                        __EMSCRIPTEN__

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
__EMSCRIPTEN_major__, __EMSCRIPTEN_minor__, __EMSCRIPTEN_tiny__

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
    #define HD_HOST_WINDOWS
#elif defined(__linux__)
    #define HD_HOST_LINUX
#elif defined(__EMSCRIPTEN__)
    #define HD_HOST_EMSCRIPTEN
#else
    #error Unknown target OS defines
#endif

/** Detect compiler */
#if defined(_MSC_VER)
    #if defined(__clang__)
        #define HD_COMPILER_CLANG_CL
    #else
        #define HD_COMPILER_MSVC
    #endif
#elif defined(__clang__)
    #if defined(__EMSCRIPTEN__)
        #define HD_COMPILER_EMSCRIPTEN
    #else
        #define HD_COMPILER_CLANG
    #endif
#elif defined(__GNUC__)
    #define HD_COMPILER_GCC
#endif

/** Detect compiler version */
#if defined(HD_COMPILER_MSVC)
    #define HD_COMPILER_VERSION _MSC_VER
#elif defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_CLANG_CL)
    #define HD_COMPILER_CLANG_VERSION_MAJOR __clang_major__
    #define HD_COMPILER_CLANG_VERSION_MINOR __clang_minor__
    #define HD_COMPILER_CLANG_VERSION_PATCH __clang_patchlevel__
    #define HD_COMPILER_VERSION HD_COMPILER_CLANG_VERSION_MAJOR, HD_COMPILER_CLANG_VERSION_MINOR, HD_COMPILER_CLANG_VERSION_PATCH
#elif defined(HD_COMPILER_GCC)
    #define HD_COMPILER_VERSION __GNUC__, __GNUC_MINOR__
#elif defined(HD_COMPILER_EMSCRIPTEN)
    #define HD_COMPILER_EMSCRIPTEN_VERSION_MAJOR __EMSCRIPTEN_major__
    #define HD_COMPILER_EMSCRIPTEN_VERSION_MINOR __EMSCRIPTEN_minor__
    #define HD_COMPILER_EMSCRIPTEN_VERSION_PATCH __EMSCRIPTEN_tiny__
    #define HD_COMPILER_VERSION HD_COMPILER_EMSCRIPTEN_VERSION_MAJOR, HD_COMPILER_EMSCRIPTEN_VERSION_MINOR, HD_COMPILER_EMSCRIPTEN_VERSION_PATCH
#else
    #error Unknown compiler version defines
#endif

/** Detect target architecture */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    #if defined(_M_X64)
        #define HD_TARGET_X64
    #elif defined(_M_IX86)
        #define HD_TARGET_X86
    #elif defined(_M_ARM64)
        #define HD_TARGET_ARM64
    #elif defined(_M_ARM)
        #define HD_TARGET_ARM32
    #endif
#elif defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)
    #if defined(__i386__)
        #define HD_TARGET_X86
    #elif defined(__x86_64__)
        #define HD_TARGET_X64
    #endif
#elif defined(HD_COMPILER_EMSCRIPTEN)
    #if defined(__LP64__)
        #define HD_TARGET_WASM64
    #else
        #define HD_TARGET_WASM32
    #endif
#else
    #error Unknown target architecture defines
#endif

#if defined(HD_TARGET_X64) || defined(HD_TARGET_ARM64) || defined(HD_TARGET_WASM64)
    #define HD_TARGET_64_BITS
#elif defined(HD_TARGET_X86) || defined(HD_TARGET_ARM32) || defined(HD_TARGET_WASM32)
    #define HD_TARGET_32_BITS
#else
    #error Unknown target architecture defines
#endif

#if defined(HD_TARGET_X64) || defined(HD_TARGET_X86)
    #define HD_TARGET_X86_FAMILY
#elif defined(HD_TARGET_ARM64) || defined(HD_TARGET_ARM32)
    #define HD_TARGET_ARM_FAMILY
#elif defined(HD_TARGET_WASM64) || defined(HD_TARGET_WASM32)
    #define HD_TARGET_WASM_FAMILY
#else
    #error Unknown target family
#endif

/** Detect endianness */
#if defined(HD_HOST_WINDOWS)
    #define HD_LITTLE_ENDIAN
#elif defined(HD_HOST_LINUX)
    #if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
        #define HD_LITTLE_ENDIAN
    #elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        #define HD_BIG_ENDIAN
    #else
        #error Unknown endianness defines
    #endif
#elif defined(HD_COMPILER_EMSCRIPTEN)
    #define HD_LITTLE_ENDIAN
#else
    #error Unknown target OS for endianness defines
#endif

/** Check required defines */
#if !defined(HD_TARGET_32_BITS) && !defined(HD_TARGET_64_BITS)
    #error Architecture targeted not supported
#endif
#if !defined(HD_COMPILER_MSVC) && !defined(HD_COMPILER_CLANG_CL) && !defined(HD_COMPILER_CLANG) && !defined(HD_COMPILER_GCC) && !defined(HD_COMPILER_EMSCRIPTEN)
    #error Compiler not supported
#endif
#if !defined(HD_HOST_WINDOWS) && !defined(HD_HOST_LINUX) && !defined(HD_HOST_EMSCRIPTEN)
    #error Host targeted not supported
#endif
#if !defined(HD_DEBUG) && !defined(HD_DEBUGOPTIMIZED) && !defined(HD_RELEASE)
    #error compilation mode is not set
#endif
#if defined(HD_COMPILER_MSVC) && !(__cplusplus >= 202002L)
    #error compilation should be done with C++ 20
#endif

#if defined(HD_TARGET_32_BITS)
static_assert(sizeof(void *) == 4, "HD_TARGET_32_BITS is defined but size of pointers are not 4 bytes");
#elif defined(HD_TARGET_64_BITS)
static_assert(sizeof(void *) == 8, "HD_TARGET_64_BITS is defined but size of pointers are not 8 bytes");
#else
    #error Target should be 32 bits or 64 bits
#endif

#endif // HD_INC_CORE_COMPILER_DEFINES_H
