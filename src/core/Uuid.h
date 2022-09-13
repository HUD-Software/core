#pragma once
#ifndef HD_INC_OSLAYER_UUID_H
#define HD_INC_OSLAYER_UUID_H

#if defined(HD_OS_WINDOWS)
#include "OSWindows/WindowsUuid.h"
#elif defined(HD_OS_LINUX)
#include "OSLinux/LinuxUuid.h"
#else
#error Targeted OS not supported
#endif

namespace hud {

    /** Provides UUID operations. */
    #if defined(HD_OS_WINDOWS)
    using Uuid = OS::Windows::Uuid;
    #elif defined(HD_OS_LINUX)
    using Uuid = OS::Linux::Uuid;
    #else
    #error Unsupported platform
    #endif

} // namespace hud 

#endif // HD_INC_OSLAYER_UUID_H