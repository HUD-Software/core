#pragma once
#ifndef HD_INC_CORE_CHAR_H
#define HD_INC_CORE_CHAR_H

#if defined(HD_OS_WINDOWS)
#include "os_windows/character.h"
#elif defined(HD_OS_LINUX)
#include "os_linux/character.h"
#else
#error Targeted OS not supported
#endif

namespace hud
{

/** Provides character operations. */
#if defined(HD_OS_WINDOWS)
    using character = os::windows::character;
#elif defined(HD_OS_LINUX)
    using character = os::linux::character;
#endif

} // namespace hud

#endif // HD_INC_CORE_CHAR_H