#pragma once
#ifndef HD_INC_OSLAYER_CHAR_H
#define HD_INC_OSLAYER_CHAR_H

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsChar.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxChar.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides character operations. */
    #if defined(HD_OS_WINDOWS)
    using Char = OS::Windows::Char;
    #elif defined(HD_OS_LINUX)
    using Char = OS::Linux::Char;
    #endif

} // namespace hud

#endif // HD_INC_OSLAYER_CHAR_H