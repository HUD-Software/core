#ifndef HD_INC_CORE_TEMPLATES_MAX_H
#define HD_INC_CORE_TEMPLATES_MAX_H
#include "less.h"

namespace hud
{

    /** Retrieves a if b < a, b otherwise. */
    template<typename type_t>
    constexpr const type_t &max(const type_t &a, const type_t &b) noexcept
    {
        return less<type_t> {}(b, a) ? a : b;
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MAX_H