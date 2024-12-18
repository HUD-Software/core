#ifndef HD_INC_CORE_DEBUGGER_DEBUGGER_BROWSER_H
#define HD_INC_CORE_DEBUGGER_DEBUGGER_BROWSER_H

#if !defined(HD_COMPILER_EMSCRIPTEN)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::browser
{
    struct debugger
    {
        /** Checks whether the calling process is being debugged by a user-mode debugger. */
        static bool is_present() noexcept;

        /** Break the debugger if the calling process is being debugged by a user-mode debugger. */
        static void break_here() noexcept;
    };

} // namespace hud::browser

#endif /* HD_INC_CORE_DEBUGGER_DEBUGGER_LINUX_H */