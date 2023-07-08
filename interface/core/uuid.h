#ifndef HD_INC_CORE_UUID_H
#define HD_INC_CORE_UUID_H

#if defined(HD_OS_WINDOWS)
    #include "os_windows/uuid.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/uuid.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

/** Provides UUID operations. */
#if defined(HD_OS_WINDOWS)
    using uuid = os::windows::uuid;
#elif defined(HD_OS_LINUX)
    using uuid = os::linux::uuid;
#else
    #error Unsupported platform
#endif

} // namespace hud

#endif // HD_INC_CORE_UUID_H