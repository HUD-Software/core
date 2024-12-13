#ifndef HD_INC_CORE_CHARACTER_CHARACTER_H
#define HD_INC_CORE_CHARACTER_CHARACTER_H

#if defined(HD_OS_WINDOWS)
    #include "character_windows.h"
#elif defined(HD_OS_LINUX)
    #include "character_linux.h"
#else
    #error Targeted OS not supported
#endif

namespace hud
{

/** Provides character operations. */
#if defined(HD_OS_WINDOWS)
    using character = hud::windows::character;
#elif defined(HD_OS_LINUX)
    using character = hud::linux::character;
#endif

} // namespace hud

#endif // HD_INC_CORE_CHARACTER_CHARACTER_H