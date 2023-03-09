#ifndef HD_INC_CORE_OS_LINUX_DEBUGGER_H
#define HD_INC_CORE_OS_LINUX_DEBUGGER_H

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct debugger
    {

        /** Checks whether the calling process is being debugged by a user-mode debugger. */
        static bool is_present() noexcept;

        /** Break the debugger if the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE void break_here() noexcept
        {
            // LCOV_EXCL_START ( We don't covert the code that break the debugger )
            if (is_present())
            {
#if defined(HD_TARGET_X86_FAMILY)
    #if HD_HAS_BUILTIN_ASM
                __asm__("int $3");
    #else
        #error hud::os::linux::debugger::break_here() is not defined
    #endif
#endif
            }
            // LCOV_EXCL_STOP
        }
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_DEBUGGER_H */