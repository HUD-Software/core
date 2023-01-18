#ifndef HD_INC_CORE_TEMPLATES_MAX_H
#define HD_INC_CORE_TEMPLATES_MAX_H
#include "less.h"

namespace hud
{

    /** Retrieves a if b < a, b otherwise. */
    template<typename u_t, typename v_t>
    constexpr auto max(const u_t &a, const v_t &b) noexcept
    {
        return less<v_t, u_t>()(b, a) ? a : b;
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MAX_H