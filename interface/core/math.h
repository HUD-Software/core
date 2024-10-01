#ifndef HD_INC_CORE_MATH_H
#define HD_INC_CORE_MATH_H

#if defined(HD_OS_WINDOWS)
    #include "os_windows/math.h"
#elif defined(HD_OS_LINUX)
    #include "os_linux/math.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{
    struct math :
#if defined(HD_OS_WINDOWS)
        os::windows::math
#elif defined(HD_OS_LINUX)
        os::linux::math
#endif
    {

    }; // namespace math

} // namespace hud
#endif // HD_INC_CORE_MATH_H