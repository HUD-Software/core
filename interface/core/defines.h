#ifndef HD_INC_CORE_DEFINES_H
#define HD_INC_CORE_DEFINES_H
#include "compiler_defines.h"
#include "builtin.h"
#include "traits/is_constant_evaluated.h"

#if defined(HD_COMPILER_GCC)
    // Use __forceinline only in non debug mode
    #if defined(HD_DEBUG)
        #define HD_FORCEINLINE
    #else
        #define HD_FORCEINLINE __attribute__((always_inline)) inline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
    #endif

    #define HD_RESTRICT __restrict__ // Indicates that a symbol is not aliased in the current scope.
    #define __STDC_WANT_LIB_EXT1__ 1 // Enable bounds-checked functions ( ISO C Safe Array Functions : memcpy_s, strcpy_s, snwprintf_s, etc... )

    #if HD_HAS_BUILTIN_UNREACHABLE
        #define HD_ASSUME(cond)                    \
            do                                     \
            {                                      \
                if (!hud::is_constant_evaluated()) \
                {                                  \
                    if (!(cond))                   \
                        __builtin_unreachable();   \
                }                                  \
            } while (false)
    #else
        #define HD_ASSUME(cond)                     \
            do                                      \
            {                                       \
                static_cast<void>(false && (cond)); \
            } while (false)
    #endif

    #if defined(HD_COMPILER_GCC) && defined(__SANITIZE_ADDRESS__)
        #if defined(__SANITIZE_ADDRESS__)
            #define HD_USE_ADDRESS_SANITIZER 1
        #endif
    #endif

    #define HD_CORE_DLL
    // Use intrinsics __int128 if __SIZEOF_INT128__ is defined
    #if defined(__SIZEOF_INT128__)
        #define HD_INTRINSIC_INT128_SUPPORTED 1
    #endif
#elif defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_EMSCRIPTEN)
    #if defined(HD_DEBUG)
        #define HD_FORCEINLINE
    #else
        #define HD_FORCEINLINE __attribute__((always_inline)) inline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
    #endif

    #define HD_RESTRICT __restrict__ // Indicates that a symbol is not aliased in the current scope.
    #define __STDC_WANT_LIB_EXT1__ 1 // Enable bounds-checked functions ( ISO C Safe Array Functions : memcpy_s, strcpy_s, snwprintf_s, etc... )

    #if HD_HAS_BUILTIN_ASSUME
        #define HD_ASSUME(cond) __builtin_assume(cond)
    #else
        #if HD_HAS_BUILTIN_UNREACHABLE
            #define HD_ASSUME(cond)              \
                do                               \
                {                                \
                    if (!(cond))                 \
                        __builtin_unreachable(); \
                } while (false)
        #else
            #define HD_ASSUME(cond)                     \
                do                                      \
                {                                       \
                    static_cast<void>(false && (cond)); \
                } while (false)
        #endif
    #endif
    #if __has_feature(address_sanitizer)
        #define HD_USE_ADDRESS_SANITIZER 1
    #endif
    #define HD_CORE_DLL
    // Use intrinsics __int128 if __SIZEOF_INT128__ is defined
    #if defined(__SIZEOF_INT128__)
        #define HD_INTRINSIC_INT128_SUPPORTED 1
    #endif

#elif defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_MSVC)
    #define HD_MS_ALIGN(bytes) __declspec(align(bytes)) // Set alignment requierement

    // Use __forceinline only in non debug mode ( With clang-cl __forceinline inline code even in Debug configuration )
    #if defined(HD_DEBUG)
        #define HD_FORCEINLINE
    #else
        #define HD_FORCEINLINE __forceinline // Overrides the cost/benefit analysis and relies on the judgment of the programmer instead
    #endif

    #define HD_RESTRICT __restrict                        // Indicates that a symbol is not aliased in the current scope.
    #define NOMINMAX                                      // Do not defines min and max macro
    #undef RSIZE_MAX                                      // MSVC deinfed RSIZE_MAX to (SIZE_MAX >> 1) in limit.h. This is C11 that is not implemented by every compiler. Use hud::cstring::RSIZE_MAX instead.
    #define HD_USE_ADDRESS_SANITIZER __SANITIZE_ADDRESS__ // The __SANITIZE_ADDRESS__ preprocessor macro is defined as 1 when /fsanitize=address is set
    #define HD_DLL_EXPORT __declspec(dllexport)           // Export a library symbol
    #define HD_DLL_IMPORT __declspec(dllimport)           // Import a library symbol
    #if defined(HD_CORE_DLL_EXPORT)
        #define HD_CORE_DLL HD_DLL_EXPORT
    #elif defined(HD_CORE_DLL_IMPORT)
        #define HD_CORE_DLL HD_DLL_IMPORT
    #else
        #define HD_CORE_DLL
    #endif
    #define HD_ASSUME(cond) __assume(cond)

#endif

#endif // HD_INC_CORE_DEFINES_H
