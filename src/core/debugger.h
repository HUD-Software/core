#pragma once
#ifndef HD_INC_CORE_DEBUGGER_H
#define HD_INC_CORE_DEBUGGER_H

#if defined(HD_OS_WINDOWS)
#include "os_windows/debugger.h"
#elif defined(HD_OS_LINUX)
#include "os_linux/debugger.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides debugger operations. */
    #if defined(HD_OS_WINDOWS)
    using debugger = os::windows::debugger;
    #elif defined(HD_OS_LINUX)
    using debugger = os::linux::debugger;
    #endif

} // namespace hud


#endif // HD_INC_CORE_DEBUGGER_H