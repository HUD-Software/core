#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_CHAR_H
#define HD_INC_OSLAYER_OS_WINDOWS_CHAR_H
#include "../OSCommon/CommonChar.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct Char 
    : public OS::Common::Char 
    {};

} // namespace hud::OS::Windows

#endif // HD_INC_OSLAYER_OS_WINDOWS_CHAR_H