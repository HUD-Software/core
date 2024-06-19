#ifndef HD_INC_CORE_OS_COMMON_MATH_H
#define HD_INC_CORE_OS_COMMON_MATH_H
#include "../traits/is_integral.h"
#include <cmath>

namespace hud::os::common
{
    struct math
    {

        /**
         * Check wheter the given number is a power of two or not
         * Requires type_t to be an integral type
         * @param value The value to test
         * @return true if value is a power of two, false otherwise
         */
        template<typename type_t>
        requires(hud::is_integral_v<type_t>)
        [[nodiscard]] static constexpr bool is_power_of_two(const type_t value) noexcept
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
        template<typename type_t>
        [[nodiscard]] static constexpr bool is_in_range_inclusive(const type_t value, const type_t min, const type_t max) noexcept
        {
            return value >= min && value <= max;
        }

        /**
         * Categorizes floating point value num into the following categories: zero, subnormal, normal, infinite, NAN.
         * @param value The value to categorize.
         * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO */
        [[nodiscard]] static constexpr int fpclassify(double value) noexcept
        {
            // FROM musl __fpclassify.c
            union
            {
                f64 f;
                u64 i;
            } u = {value};

            int e = u.i >> 52 & 0x7ff;
            if (!e)
                return u.i << 1 ? FP_SUBNORMAL : FP_ZERO;
            if (e == 0x7ff)
                return u.i << 12 ? FP_NAN : FP_INFINITE;
            return FP_NORMAL;
        }

        /**
         * Categorizes floating point value num into the following categories: zero, subnormal, normal, infinite, NAN.
         * @param value The value to categorize.
         * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO */
        [[nodiscard]] static constexpr int fpclassify(float value) noexcept
        {
            // FROM musl __fpclassifyf.c
            union
            {
                f32 f;
                u32 i;
            } u = {value};

            int e = u.i >> 23 & 0xff;
            if (!e)
                return u.i << 1 ? FP_SUBNORMAL : FP_ZERO;
            if (e == 0xff)
                return u.i << 9 ? FP_NAN : FP_INFINITE;
            return FP_NORMAL;
        }

        /**
         * Check if the given floating point number has finite value (not infinite or NaN)
         * @param value The value to test
         * @return true if value if finite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_finite(const float value) noexcept
        {
            return std::isfinite(value);
        }

        /**
         * Check if the given floating point number has finite value (not infinite or NaN)
         * @param value The value to test
         * @return true if value if finite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_finite(const double value) noexcept
        {
            return std::isfinite(value);
        }

        /** Check if the given floating point number is a infinite value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_infinite(const float value) noexcept
        {
            return std::isinf(value);
        }

        /** Check if the given floating point number is a infinite value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_infinite(const double value) noexcept
        {
            return std::isinf(value);
        }

        /** Check if the given floating point number is a NaN value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_nan(const float value) noexcept
        {
            return std::isnan(value);
        }

        /** Check if the given floating point number is a normal value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_nan(const double value) noexcept
        {
            return std::isnan(value);
        }

        /** Check if the given floating point number is a normal value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_normal(const float value) noexcept
        {
            return std::isnormal(value);
        }

        /** Check if the given floating point number is a normal value
         * @param vlaue The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_normal(const double value) noexcept
        {
            return std::isnormal(value);
        }

        /**
         * Multiplies a floating point value by the number 2 raised to the exp power.
         * @param value The value to compute
         * @param exp The exponent
         * @return value*(2^exp)
         */
        [[nodiscard]] static constexpr f32 ldexp(f32 value, i32 exp) noexcept
        {
            return std::ldexp(value, exp);
        }

        /**
         * Multiplies a floating point value by the number 2 raised to the exp power.
         * @param value The value to compute
         * @param exp The exponent
         * @return value*(2^exp)
         */
        [[nodiscard]] static constexpr f64 ldexp(f64 value, i32 exp) noexcept
        {
            return std::ldexp(value, exp);
        }
    };
} // namespace hud::os::common
#endif // HD_INC_CORE_OS_COMMON_MATH_H