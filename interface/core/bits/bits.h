#ifndef HD_INC_CORE_BITS_BITS_H
#define HD_INC_CORE_BITS_BITS_H

#if defined(HD_OS_WINDOWS)
    #include "bits_windows.h"
#elif defined(HD_OS_LINUX) || defined(HD_OS_BROWSER)
    #include "bits_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{
    /** Provides memory operations. */
    struct bits :
#if defined(HD_OS_WINDOWS)
        hud::windows::bits
#elif defined(HD_OS_LINUX) || defined(HD_OS_BROWSER)
        hud::linux::bits
#endif
    {
    };
} // namespace hud
#endif // HD_INC_CORE_BITS_BITS_H