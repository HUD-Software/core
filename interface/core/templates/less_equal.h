#ifndef HD_INC_CORE_TEMPLATES_LESS_EQUAL_H
#define HD_INC_CORE_TEMPLATES_LESS_EQUAL_H

namespace hud
{

    /** Checks whether the first argument of type type_t compares less than or equal to the second (as returned by operator <=). */
    template<typename type_t>
    struct less_equal
    {
        /** Member function returning whether the first argument compares less than or equal to the second (a<=b). */
        [[nodiscard]] constexpr bool operator()(const type_t &a, const type_t &b) const noexcept
        {
            return a <= b;
        }
    };

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_LESS_EQUAL_H
