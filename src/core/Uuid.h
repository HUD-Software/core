#pragma once
#ifndef HD_INC_OSLAYER_UUID_H
#define HD_INC_OSLAYER_UUID_H

#if defined(HD_OS_WINDOWS)
#include "os_windows/uuid.h"
#elif defined(HD_OS_LINUX)
#include "os_linux/uuid.h"
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