#pragma once
#ifndef HD_INC_OSLAYER_OS_LINUX_TYPES_H
#define HD_INC_OSLAYER_OS_LINUX_TYPES_H
#include "../OSCommon/CommonTypes.h"

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif 

namespace hud::OS::Linux{

    struct Types : public OS::Common::Types {
        static inline constexpr wchar wchar_max = i32_max;
        static inline constexpr wchar wchar_min = i32_min;
    };

} //namespace hud::OS::Linux

#endif // HD_INC_OSLAYER_OS_LINUX_TYPES_H