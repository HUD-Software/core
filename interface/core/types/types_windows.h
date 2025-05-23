#ifndef HD_INC_CORE_TYPES_TYPES_WINDOWS_H
#define HD_INC_CORE_TYPES_TYPES_WINDOWS_H
#include "types_common.h"

#if !defined(HD_HOST_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::windows
{

    struct types : public hud::common::types
    {
        static inline constexpr wchar wchar_max = u16_max;
        static inline constexpr wchar wchar_min = u16_min;
        static inline constexpr wchar wchar_min_positive = u16_min_positive;
    };

} // namespace hud::windows

#endif // HD_INC_CORE_TYPES_TYPES_WINDOWS_H