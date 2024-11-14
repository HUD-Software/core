#ifndef HD_INC_CORE_OS_LINUX_MEMORY_H
#define HD_INC_CORE_OS_LINUX_MEMORY_H
#include "../os_common/memory.h"

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct memory : public os::common::memory
    {
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_MEMORY_H */