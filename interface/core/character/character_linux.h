#ifndef HD_INC_CORE_CHARACTER_CHARACTER_LINUX_H
#define HD_INC_CORE_CHARACTER_CHARACTER_LINUX_H
#include "character_linux.h"

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::linux
{

    struct character
        : public hud::common::character
    {
    };

} // namespace hud::linux

#endif // HD_INC_CORE_CHARACTER_CHARACTER_LINUX_H