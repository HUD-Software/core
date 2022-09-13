#pragma once
#ifndef HD_INC_OSLAYER_OS_LINUX_DEBUGGER_H
#define HD_INC_OSLAYER_OS_LINUX_DEBUGGER_H

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif 

namespace hud::OS::Linux{

    struct Debugger {

        /** Checks whether the calling process is being debugged by a user-mode debugger. */
        static bool is_present() noexcept;

        /**
        * Causes a breakpoint exception to occur in the current process if condition is false.
        * This allows the calling thread to signal the debugger to handle the exception.
        * Not breaking if debugger is not present.
        * @param condition The condition to test
        */
        static void check(const bool condition) noexcept {
            if (condition == false) {
                break_here();
                #if defined HD_ABORT_ON_ASSERT
                abort();
                #endif
            }
        }

        /** Break the debugger if the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE void break_here() noexcept {
            if (is_present()) {
               if constexpr(hud::Compilation::is_compiler(ECompiler::clang)) {
                    __builtin_debugtrap();
               } else {
                    #if defined(HD_TARGET_X86_FAMILY)
                    __asm__("int $3");
                    #elif defined(HD_TARGET_ARM_FAMILY)
                    __builtin_trap();
                    #endif
               }
            }
        }

    };

} //namespace hud::OS::Windows

#endif /* HD_INC_OSLAYER_OS_LINUX_DEBUGGER_H */ 