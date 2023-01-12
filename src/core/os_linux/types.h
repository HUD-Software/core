#pragma once
#ifndef HD_INC_CORE_OS_LINUX_TYPES_H
#define HD_INC_CORE_OS_LINUX_TYPES_H
#include "../os_common/types.h"

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct types : public os::common::types
    {
        static inline constexpr wchar wchar_max = i32_max;
        static inline constexpr wchar wchar_min = i32_min;
    };

} // namespace hud::os::linux

#endif // HD_INC_CORE_OS_LINUX_TYPES_H