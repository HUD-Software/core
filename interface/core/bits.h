#ifndef HD_INC_CORE_BITS_H
#define HD_INC_CORE_BITS_H
#include "bits.h"

#if defined(HD_OS_WINDOWS)
    #include "os_windows/bits.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/bits.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

    /** Provides memory operations. */
    struct bits :
#if defined(HD_OS_WINDOWS)
        os::windows::bits
#elif defined(HD_OS_LINUX)
        os::linux::bits
#endif
    {
    };
} // namespace hud
#endif // HD_INC_CORE_BITS_H