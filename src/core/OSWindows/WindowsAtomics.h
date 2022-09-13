#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_H
#define HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_H

#if defined(HD_TARGET_X86) |  defined(HD_TARGET_X64)
#include "WindowsAtomicsIntel.h"
#endif

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif


namespace hud::OS::Windows{
    
#if defined(HD_TARGET_X86) |  defined(HD_TARGET_X64)
    struct Atomics : public AtomicsIntel{};
#endif

} // namespace hud

#endif // HD_INC_OSLAYER_OS_WINDOWS_ATOMICS_H