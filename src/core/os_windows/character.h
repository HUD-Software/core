#pragma once
#ifndef HD_INC_CORE_OS_WINDOWS_CHAR_H
#define HD_INC_CORE_OS_WINDOWS_CHAR_H
#include "../os_common/char.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::os::windows{

    struct character 
    : public os::common::character 
    {};

} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_CHAR_H