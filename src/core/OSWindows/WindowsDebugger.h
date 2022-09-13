#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_DEBUGGER_H
#define HD_INC_OSLAYER_OS_WINDOWS_DEBUGGER_H
#include "WindowsAPI.h"
#include <cstdlib> // abort

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct Debugger {

        /** Checks whether the calling process is being debugged by a user-mode debugger. */
        static HD_FORCEINLINE bool is_present() noexcept {
            return ::IsDebuggerPresent() != 0;
        }

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
                ::DebugBreak();
            }
        }

    };

} //namespace hud::OS::Windows

#endif // HD_INC_OSLAYER_OS_WINDOWS_DEBUGGER_H