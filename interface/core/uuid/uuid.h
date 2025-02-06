#ifndef HD_INC_CORE_UUID_UUID_H
#define HD_INC_CORE_UUID_UUID_H

#if defined(HD_HOST_WINDOWS)
    #include "uuid_windows.h"
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
    #include "uuid_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

/** Provides UUID operations. */
#if defined(HD_HOST_WINDOWS)
    using uuid = hud::windows::uuid;
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
    using uuid = hud::linux::uuid;
#else
    #error Unsupported platform
#endif

} // namespace hud

#endif // HD_INC_CORE_UUID_UUID_H