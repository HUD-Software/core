#ifndef HD_INC_CORE_TEMPLATES_LESS_H
#define HD_INC_CORE_TEMPLATES_LESS_H

namespace hud
{

    /** Checks whether the first argument of type type_t compares less than the second (as returned by operator <). */
    template<typename type_t, typename u_type_t = type_t>
    struct less
    {
        /** Member function returning whether the first argument compares less than the second (a<b). */
        [[nodiscard]] constexpr bool operator()(const type_t &lhs, const u_type_t &rhs) const noexcept
        {
            return lhs < rhs;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_LESS_H
