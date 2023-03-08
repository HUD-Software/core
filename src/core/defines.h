#ifndef HD_INC_CORE_DEFINES_H
#define HD_INC_CORE_DEFINES_H
#include "compiler_defines.h"

#if defined(HD_OS_WINDOWS)
    #if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)

        #define HD_MS_ALIGN(bytes) __declspec(align(bytes)) // Set alignment requierement

        // Use __forceinline only in non debug mode ( With clang-cl __forceinline inline code even in Debug configuration )
        #if defined(HD_DEBUG)
            #define HD_FORCEINLINE
        #else
            #define HD_FORCEINLINE __forceinline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
        #endif

        #define HD_RESTRICT __restrict                     // Indicates that a symbol is not aliased in the current scope.
        #define NOMINMAX                                   // Do not defines min and max macro
        #undef RSIZE_MAX                                   // MSVC deinfed RSIZE_MAX to (SIZE_MAX >> 1) in limit.h. This is C11 that is not implemented by every compiler. Use hud::cstring::RSIZE_MAX instead.
        #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO 1         // MSVC and Clang-cl defined __is_convertible_to builtin
        #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE 1 // MSVC and Clang-cl defined __is_trivially_destructible
        #if defined(HD_COMPILER_MSVC)
            #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 0       // MSVC don't have __builtin_bswap32 builtin
            #define HD_HAS_BUILTIN_BULITIN_BSWAP64 0       // MSVC don't have __builtin_bswap64 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 0  // MSVC don't have __builtin_rotateleft32 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 0  // MSVC don't have __builtin_rotateleft64 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 0 // MSVC don't have __builtin_rotateright32 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 0 // MSVC don't have __builtin_rotateright64 builtin
            #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE 0 // MSVC don't have __atomic_thread_fence builtin
        #elif defined(HD_COMPILER_CLANG_CL)
            #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)             // Clang and GCC maybe defined __builtin_bswap32 builtin
            #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)             // Clang and GCC maybe defined __builtin_bswap64 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)   // Clang and GCC maybe defined __builtin_rotateleft32 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)   // Clang and GCC maybe defined __builtin_rotateleft64 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32) // Clang and GCC maybe defined __builtin_rotateright32 builtin
            #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64) // Clang and GCC maybe defined __builtin_rotateright64 builtin
            #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)   // Clang and GCC maybe defined __atomic_thread_fence builtin
        #endif
    #else
        #error Unsupported compiler for Windows
    #endif // HD_COMPILER_MSVC || HD_COMPILER_CLANG_CL

#elif defined(HD_OS_LINUX)

    #if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)

        // Use __forceinline only in non debug mode
        #if defined(HD_DEBUG)
            #define HD_FORCEINLINE
        #else
            #define HD_FORCEINLINE __attribute__((always_inline)) inline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
        #endif

        #define HD_RESTRICT __restrict__ // Indicates that a symbol is not aliased in the current scope.
        #define __STDC_WANT_LIB_EXT1__ 1 // Enable bounds-checked functions ( ISO C Safe Array Functions : memcpy_s, strcpy_s, snwprintf_s, etc... )

        #if defined(HD_COMPILER_CLANG)
            #if __has_feature(address_sanitizer)
                #define ADDRESS_SANITIZER
            #endif
        #elif defined(HD_COMPILER_GCC) && defined(__SANITIZE_ADDRESS__)
            #if defined(__SANITIZE_ADDRESS__)
                #define ADDRESS_SANITIZER
            #endif

        #endif

        #define HD_HAS_BUILTIN_IS_CONVERTIBLE_TO __has_builtin(__is_convertible_to)                 // Clang and GCC maybe defined __is_convertible_to builtin
        #define HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE __has_builtin(__is_trivially_destructible) // Clang and GCC maybe defined __is_trivially_destructible builtin
        #define HD_HAS_BUILTIN_BUILTIN_BSWAP32 __has_builtin(__builtin_bswap32)                     // Clang and GCC maybe defined __builtin_bswap32 builtin
        #define HD_HAS_BUILTIN_BULITIN_BSWAP64 __has_builtin(__builtin_bswap64)                     // Clang and GCC maybe defined __builtin_bswap64 builtin
        #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT32 __has_builtin(__builtin_rotateleft32)           // Clang and GCC maybe defined __builtin_rotateleft32 builtin
        #define HD_HAS_BUILTIN_BULITIN_ROTATELEFT64 __has_builtin(__builtin_rotateleft64)           // Clang and GCC maybe defined __builtin_rotateleft64 builtin
        #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32 __has_builtin(__builtin_rotateright32)         // Clang and GCC maybe defined __builtin_rotateright32 builtin
        #define HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64 __has_builtin(__builtin_rotateright64)         // Clang and GCC maybe defined __builtin_rotateright64 builtin
        #define HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE __has_builtin(__atomic_thread_fence)             // Clang and GCC maybe defined __atomic_thread_fence builtin

    #else
        #error Unsupported compiler for Linux
    #endif // defined(HD_COMPILER_CLANG)

#else
    #error Unsupported operating system

#endif // defined(HD_OS_...)

/** Check that builtin checks is complete */
#ifndef HD_HAS_BUILTIN_IS_CONVERTIBLE_TO
    #error has __is_convertible_to builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_IS_TRIVIALLY_DESTRUCTIBLE
    #error has __is_trivially_destructible builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BUILTIN_BSWAP32
    #error has __builtin_bswap32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_BSWAP64
    #error has __builtin_bswap64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATELEFT32
    #error has __builtin_rotateleft32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATELEFT64
    #error has __builtin_rotateleft64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32
    #error has __builtin_rotateright32 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64
    #error has __builtin_rotateright64 builtin is not defined
#endif
#ifndef HD_HAS_BUILTIN_ATOMIC_THREAD_FENCE
    #error has __atomic_thread_fence builtin is not defined
#endif
#endif // HD_INC_CORE_DEFINES_H
