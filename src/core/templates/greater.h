#pragma once
#ifndef HD_INC_CORE_TEMPLATES_GREATER_H
#define HD_INC_CORE_TEMPLATES_GREATER_H

namespace hud
{

    /** Checks whether the first argument of type type_t compares greater than the second (as returned by operator >). */
    template <typename type_t>
    struct greater
    {
        /** Member function returning whether the first argument compares greater than the second (a>b). */
        [[nodiscard]] constexpr bool operator()(const type_t &lhs, const type_t &rhs) const noexcept
        {
            return lhs > rhs;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_GREATER_H
