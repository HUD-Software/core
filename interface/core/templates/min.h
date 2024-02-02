#ifndef HD_INC_CORE_TEMPLATES_MIN_H
#define HD_INC_CORE_TEMPLATES_MIN_H
#include "less.h"

namespace hud
{

    /** Retrieves a if a < b, b otherwise. */
    template<typename type_t>
    constexpr const type_t &min(const type_t &a, const type_t &b) noexcept
    {
        return less<type_t> {}(a, b) ? a : b;
    }

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_MIN_H