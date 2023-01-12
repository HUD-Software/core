#pragma once
#ifndef HD_INC_CORE_OS_LINUX_CHAR_H
#define HD_INC_CORE_OS_LINUX_CHAR_H
#include "../os_common/character.h"

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct character
        : public os::common::character
    {
    };

} // namespace hud::os::linux

#endif // HD_INC_CORE_OS_LINUX_CHAR_H