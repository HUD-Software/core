#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_TYPES_H
#define HD_INC_OSLAYER_OS_WINDOWS_TYPES_H
#include "../OSCommon/CommonTypes.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct Types : public OS::Common::Types {
        static inline constexpr wchar wchar_max = u16_max;
        static inline constexpr wchar wchar_min = u16_min;
    };

} //namespace hud::OS::Windows

#endif // HD_INC_OSLAYER_OS_WINDOWS_TYPES_H