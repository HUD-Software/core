#ifndef HD_INC_CORE_TEMPLATES_MIN_H
#define HD_INC_CORE_TEMPLATES_MIN_H
#include "less.h"

namespace hud
{

    /** Retrieves a if a < b, b otherwise. */
    template<typename type_t>
    constexpr type_t &&min(type_t &&a, type_t &&b) noexcept
    {
        return less<type_t, type_t>()(a, b) ? hud::forward<type_t &&>(a) : hud::forward<type_t &&>(b);
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MIN_H