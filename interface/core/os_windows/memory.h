#ifndef HD_INC_CORE_OS_WINDOWS_MEMORY_H
#define HD_INC_CORE_OS_WINDOWS_MEMORY_H
#include "../os_common/memory.h"

#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::os::windows
{

    struct memory : public os::common::memory
    {
    };

} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_MEMORY_H
