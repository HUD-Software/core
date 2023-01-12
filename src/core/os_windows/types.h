#pragma once
#ifndef HD_INC_CORE_OS_WINDOWS_TYPES_H
#define HD_INC_CORE_OS_WINDOWS_TYPES_H
#include "../os_common/types.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif

namespace hud::os::windows
{

    struct types : public os::common::types
    {
        static inline constexpr wchar wchar_max = u16_max;
        static inline constexpr wchar wchar_min = u16_min;
    };

} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_TYPES_H