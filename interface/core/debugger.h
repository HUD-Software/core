#ifndef HD_INC_CORE_DEBUGGER_H
#define HD_INC_CORE_DEBUGGER_H

#if defined(HD_OS_WINDOWS)
    #include "os_windows/debugger.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/debugger.h"
    #include <stdlib.h> // abort
#else
    #error Targeted OS not supported
#endif

namespace hud
{

/** Provides debugger operations. */
#if defined(HD_OS_WINDOWS)
    struct debugger : os::windows::debugger
#elif defined(HD_OS_LINUX)
    struct debugger : os::linux::debugger
#endif
    {
        /** Break the debugger if condition is false and if the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE void break_here_if(const bool condition) noexcept
        {
            if (condition)
            {
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
            if (condition)
            {
                abort();
            }
        } // LCOV_EXCL_STOP
    };
} // namespace hud

#endif // HD_INC_CORE_DEBUGGER_H