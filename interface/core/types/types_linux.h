#ifndef HD_INC_CORE_TYPES_TYPES_LINUX_H
#define HD_INC_CORE_TYPES_TYPES_LINUX_H
#include "types_common.h"

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::linux
{

    struct types
        : public hud::common::types
    {
        static inline constexpr wchar wchar_max = i32_max;
        static inline constexpr wchar wchar_min = i32_min;
        static inline constexpr wchar wchar_min_positive = i32_min_positive;
    };

} // namespace hud::linux

#endif // HD_INC_CORE_TYPES_TYPES_LINUX_H