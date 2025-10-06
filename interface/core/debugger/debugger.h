#ifndef HD_INC_CORE_DEBUGGER_DEBUGGER_H
#define HD_INC_CORE_DEBUGGER_DEBUGGER_H

#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_MSVC)
#include "debugger_windows.h"
#elif defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)
#include "debugger_linux.h"
#include <stdlib.h> // abort
#elif defined(HD_COMPILER_EMSCRIPTEN)
#include "debugger_browser.h"
#include <stdlib.h> // abort
#else
#error Targeted OS not supported
#endif

namespace hud
{

/** Provides debugger operations. */
#if defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_MSVC)
    struct debugger : hud::windows::debugger
#elif defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_GCC)
    struct debugger : hud::linux::debugger
#elif defined(HD_COMPILER_EMSCRIPTEN)
    struct debugger : hud::browser::debugger
#endif
    {
        /** Break the debugger if condition is false and if the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE void break_here_if(const bool condition) noexcept
        {
            if (condition) [[unlikely]] {
                break_here();
            }
        }

        /** Abort the program. */
        // LCOV_EXCL_START ( We don't covert the abort )
        static HD_FORCEINLINE void abort() noexcept
        {
            ::abort();

        } // LCOV_EXCL_STOP

        /** Abort the program if condition is true. */
        // LCOV_EXCL_START ( We don't covert the abort )
        static HD_FORCEINLINE void abort_if(const bool condition) noexcept
        {
            if (condition) [[unlikely]] {
                abort();
            }
        } // LCOV_EXCL_STOP
    };
} // namespace hud

#endif // HD_INC_CORE_DEBUGGER_H