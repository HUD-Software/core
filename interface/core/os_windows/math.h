#ifndef HD_INC_CORE_OS_WINDOWS_MATH_H
#define HD_INC_CORE_OS_WINDOWS_MATH_H
#include "../os_common/math.h"
#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::os::windows
{
    struct math
        : public os::common::math
    {
    };
} // namespace hud::os::windows

#endif /* HD_INC_CORE_OS_WINDOWS_MATH_H */