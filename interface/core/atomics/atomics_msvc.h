#ifndef HD_INC_CORE_ATOMICS_ATOMICS_MSVC_H
#define HD_INC_CORE_ATOMICS_ATOMICS_MSVC_H

#if defined(HD_TARGET_X86) | defined(HD_TARGET_X64)
    #include "atomics_msvc_intel.h"
#endif

#if !defined(HD_HOST_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif

namespace hud::msvc
{

#if defined(HD_TARGET_X86) | defined(HD_TARGET_X64)
    struct atomics
        : public atomics_intel
    {
    };
#endif

} // namespace hud::msvc

#endif // HD_INC_CORE_ATOMICS_ATOMICS_MSVC_H