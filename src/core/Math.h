#pragma once
#ifndef HD_INC_OSLAYER_MATH_H
#define HD_INC_OSLAYER_MATH_H
#include "Traits/MakeUnsigned.h"
#include "Traits/IsIntegral.h"

namespace hud {
    struct Math {
      
        /**
        * Check wheter the given number is a power of two or not
        * Requires T to be an integral type
        * @param value The value to test
        * @return true if value is a power of two, false otherwise
        */
        template<typename T>
        [[nodiscard]]
        static constexpr bool is_power_of_two(const T value) noexcept requires(IsIntegralV<T>)
        {
            return value && !(value & (value - 1));
        }

        /**
        * Check that a value is in range [min, max]
        * @param value The value to check
        * @param min The minimum value of the range
        * @param max The maximum value of the range
        * @return true if value >= min and value <= max, false otherwise
        */
        template<typename T>
        [[nodiscard]]
        static constexpr bool is_in_range_inclusive(const T value, const T min, const T max) noexcept {
            return value >= min && value <= max;
        }
    };
} // namespace hud
#endif // HD_INC_OSLAYER_MATH_H