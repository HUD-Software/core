#ifndef HD_INC_CORE_TEMPLATES_EQUAL_H
#define HD_INC_CORE_TEMPLATES_EQUAL_H

namespace hud
{

    /** Checks whether its two arguments of type type_t compare equal (as returned by operator ==). */
    template<typename type_t>
    struct equal
    {
        /** Member function returning whether the arguments compare equal (lhs==rhs). */
        [[nodiscard]] constexpr bool operator()(const type_t &lhs, const type_t &rhs) const noexcept
        {
            return lhs == rhs;
        }
    };

    /** Checks whether lhs_t type is comparable with rhs_t type with operator==. */
    template<typename lhs_t, typename rhs_t, typename = void>
    struct is_comparable_with_equal
        : hud::false_type
    {
    };

    template<typename lhs_t, typename rhs_t>
    struct is_comparable_with_equal<lhs_t, rhs_t, void_t<decltype(hud::equal<lhs_t> {}(hud::declval<hud::add_const_t<hud::add_lvalue_reference_t<lhs_t>>>(), hud::declval<hud::add_const_t<hud::add_lvalue_reference_t<rhs_t>>>()))>>
        : hud::true_type
    {
    };

    /** Equivalent of is_comparable_with_equal<lhs_t,rhs_t>::value. */
    template<typename lhs_t, typename rhs_t>
    inline constexpr bool is_comparable_with_equal_v = is_comparable_with_equal<lhs_t, rhs_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TEMPLATES_EQUAL_H