#pragma once
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
            if (is_present())
            {
                if constexpr (hud::compilation::is_compiler(compiler_e::clang))
                {
                    __builtin_debugtrap();
                }
                else
                {
#if defined(HD_TARGET_X86_FAMILY)
                    __asm__("int $3");
#elif defined(HD_TARGET_ARM_FAMILY)
                    __builtin_trap();
#endif
                }
            }
        }
    };

} // namespace hud::os::windows

#endif /* HD_INC_CORE_OS_LINUX_DEBUGGER_H */