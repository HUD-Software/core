#ifndef HD_INC_CORE_TRAITS_INTEGRAL_CONSTANT_H
#define HD_INC_CORE_TRAITS_INTEGRAL_CONSTANT_H

namespace hud
{

    /** Wraps a static constant value of specified type type_t. */
    template<typename type_t, type_t type_t_value>
    struct integral_constant
    {
        /** Wrapped static constexpr value. */
        static constexpr type_t value = type_t_value;
        /** Type of the wrapped value. */
        using value_type = type_t;
        /** Type of the hud::integral_constant. */
        using type = integral_constant;

        /** Conversion function. Returns the wrapped value. */
        constexpr operator value_type() const noexcept
        {
            return value;
        }

        /** Conversion function. Returns the wrapped value. */
        constexpr value_type operator()() const noexcept
        {
            return value;
        }
    };

    /** hud::integral_constant wraps a static constant boolean value. */
    template<bool value>
    using bool_constant = hud::integral_constant<bool, value>;

    /** True boolean integral constant */
    using true_type = hud::bool_constant<true>;

    /** False boolean integral constant */
    using false_type = hud::bool_constant<false>;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_INTEGRAL_CONSTANT_H