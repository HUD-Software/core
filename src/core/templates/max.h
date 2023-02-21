#ifndef HD_INC_CORE_TEMPLATES_MAX_H
#define HD_INC_CORE_TEMPLATES_MAX_H
#include "less.h"

namespace hud
{

    /** Retrieves a if b < a, b otherwise. */
    template<typename type_t>
    constexpr auto max(type_t &&a, type_t &&b) noexcept
    {
        return less<type_t, type_t>()(b, a) ? hud::forward<type_t &&>(a) : hud::forward<type_t &&>(b);
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MAX_H