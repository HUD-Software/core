#pragma once
#ifndef HD_INC_CORE_TEMPLATES_EQUAL_H
#define HD_INC_CORE_TEMPLATES_EQUAL_H

namespace hud
{

    /** Checks whether its two arguments of type type_t compare equal (as returned by operator ==). */
    template <typename type_t>
    struct equal
    {
        /** Member function returning whether the arguments compare equal (lhs==rhs). */
        [[nodiscard]] constexpr bool operator()(const type_t &lhs, const type_t &rhs) const noexcept
        {
            return lhs == rhs;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_EQUAL_H