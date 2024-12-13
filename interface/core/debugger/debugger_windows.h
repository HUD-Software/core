#ifndef HD_INC_CORE_DEBUGGER_DEBUGGER_WINDOWS_H
#define HD_INC_CORE_DEBUGGER_DEBUGGER_WINDOWS_H
#include "../os_windows/windows_api.h"
#include <cstdlib> // abort

#if !defined(HD_COMPILER_MSVC) && !defined(HD_COMPILER_CLANG_CL)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::windows
{
    struct debugger
    {
        // LCOV_EXCL_START ( We don't covert the code that break the debugger )

        /** Checks whether the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE bool is_present() noexcept
        {
            return ::IsDebuggerPresent() != 0;
        }

        /** Break the debugger if the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE void break_here() noexcept
        {
            if (is_present())
            {
                ::DebugBreak();
            }
        }

        // LCOV_EXCL_STOP
    };

} // namespace hud::windows

#endif // HD_INC_CORE_DEBUGGER_DEBUGGER_WINDOWS_H