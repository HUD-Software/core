#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_INTEGRAL_CONSTANT_H
#define HD_INC_OSLAYER_TRAITS_INTEGRAL_CONSTANT_H

namespace hud {

    /** Wraps a static constant value of specified type T. */
    template<typename T, T value>
    struct IntegralConstant {
        /** Wrapped static constexpr value. */
        static constexpr T Value = value;
        /** Type of the wrapped value. */
        using ValueType = T;
        /** Type of the IntegralConstant. */
        using Type = IntegralConstant;

        /** Conversion function. Returns the wrapped value. */
        constexpr operator ValueType() const noexcept {
            return Value;
        }
        /** Conversion function. Returns the wrapped value. */
        constexpr ValueType operator()() const noexcept {
            return Value;
        }
    };

    /** IntegralConstant wraps a static constant boolean value. */
    template<bool value>
    using BoolConstant = IntegralConstant<bool, value>;

    /** True boolean integral constant */
    using TrueType = BoolConstant<true>;

    /** False boolean integral constant */
    using FalseType = BoolConstant<false>;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_INTEGRAL_CONSTANT_H