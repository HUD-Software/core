#pragma once
#ifndef HD_INC_OSLAYER_OS_LINUX_CHAR_H
#define HD_INC_OSLAYER_OS_LINUX_CHAR_H
#include "../OSCommon/CommonChar.h"

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif 


namespace hud::OS::Linux{

    struct Char 
        : public OS::Common::Char 
    {};

} // namespace hud::OS::Linux

#endif // HD_INC_OSLAYER_OS_LINUX_CHAR_H