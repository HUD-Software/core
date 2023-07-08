#ifndef HD_INC_CORE_DEFINES_H
#define HD_INC_CORE_DEFINES_H
#include "compiler_defines.h"
#include "builtin.h"

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
        #define USE_ADDRESS_SANITIZER __SANITIZE_ADDRESS__ // The __SANITIZE_ADDRESS__ preprocessor macro is defined as 1 when /fsanitize=address is set
    #else
        #error Unsupported compiler for Windows
    #endif

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
                #define USE_ADDRESS_SANITIZER 1
            #endif
        #elif defined(HD_COMPILER_GCC) && defined(__SANITIZE_ADDRESS__)
            #if defined(__SANITIZE_ADDRESS__)
                #define USE_ADDRESS_SANITIZER 1
            #endif
        #endif
    #else
        #error Unsupported compiler for Linux
    #endif

#else
    #error Unsupported operating system

#endif

#endif // HD_INC_CORE_DEFINES_H
