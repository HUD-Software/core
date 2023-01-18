#ifndef HD_INC_CORE_OS_WINDOWS_DEBUGGER_H
#define HD_INC_CORE_OS_WINDOWS_DEBUGGER_H
#include "windows_api.h"
#include <cstdlib> // abort

#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::os::windows
{

    struct debugger
    {

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
    };

} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_DEBUGGER_H