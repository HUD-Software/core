#ifndef HD_INC_CORE_UUID_UUID_H
#define HD_INC_CORE_UUID_UUID_H

#if defined(HD_OS_WINDOWS)
    #include "uuid_windows.h"
#elif defined(HD_OS_LINUX) || defined(HD_OS_EMSCRIPTEN)
    #include "uuid_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

/** Provides UUID operations. */
#if defined(HD_OS_WINDOWS)
    using uuid = hud::windows::uuid;
#elif defined(HD_OS_LINUX) || defined(HD_OS_EMSCRIPTEN)
    using uuid = hud::linux::uuid;
#else
    #error Unsupported platform
#endif

} // namespace hud

#endif // HD_INC_CORE_UUID_UUID_H