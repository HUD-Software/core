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
                // With clang/gcc we can break the debugger on x86 by invoking int3
#if defined(HD_TARGET_X86_FAMILY)

                asm("int $3");
#else
                // Dereference nullptr through a volatile pointer to prevent the compiler
                // from removing. We use this rather than abort() or __builtin_trap() for
                // portability: some debuggers don't correctly trap abort().
                *static_cast<volatile int *>(nullptr) = 1;
#endif
            }
            // LCOV_EXCL_STOP
        }
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_DEBUGGER_H */