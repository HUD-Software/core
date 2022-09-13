#pragma once
#ifndef HD_INC_OSLAYER_DEBUGGER_H
#define HD_INC_OSLAYER_DEBUGGER_H

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsDebugger.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxDebugger.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides Debugger operations. */
    #if defined(HD_OS_WINDOWS)
    using Debugger = OS::Windows::Debugger;
    #elif defined(HD_OS_LINUX)
    using Debugger = OS::Linux::Debugger;
    #endif

} // namespace hud


#endif // HD_INC_OSLAYER_DEBUGGER_H