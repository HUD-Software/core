#ifndef HD_INC_CORE_CHARACTER_CHARACTER_WINDOWS_H
#define HD_INC_CORE_CHARACTER_CHARACTER_WINDOWS_H
#include "character_common.h"

#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::windows
{

    struct character
        : public hud::common::character
    {
    };

} // namespace hud::windows

#endif // HD_INC_CORE_CHARACTER_CHARACTER_WINDOWS_H