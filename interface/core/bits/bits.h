#ifndef HD_INC_CORE_BITS_BITS_H
#define HD_INC_CORE_BITS_BITS_H

#if defined(HD_HOST_WINDOWS)
    #include "bits_windows.h"
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
    #include "bits_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{
    /** Provides memory operations. */
    struct bits :
#if defined(HD_HOST_WINDOWS)
        hud::windows::bits
#elif defined(HD_HOST_LINUX) || defined(HD_HOST_EMSCRIPTEN)
        hud::linux::bits
#endif
    {
    };
} // namespace hud
#endif // HD_INC_CORE_BITS_BITS_H