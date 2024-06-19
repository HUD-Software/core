#ifndef HD_INC_CORE_MATH_H
#define HD_INC_CORE_MATH_H
#include "traits/make_unsigned.h"
#include "traits/is_integral.h"

namespace hud
{
    namespace math
    {
        /**
         * Defines limits of types
         * @tparam type_t The type to define
         */
        template<typename type_t>
        struct limits;
#define hud_limits_for(type_t)                           \
    template<> struct limits<type_t>                     \
    {                                                    \
        static constexpr type_t min {hud::type_t##_min}; \
        static constexpr type_t max {hud::type_t##_max}; \
    };
        hud_limits_for(bool);
        hud_limits_for(i8);
        hud_limits_for(u8);
        hud_limits_for(i16);
        hud_limits_for(u16);
        hud_limits_for(i32);
        hud_limits_for(u32);
        hud_limits_for(i64);
        hud_limits_for(u64);
#undef hud_limits_for

        /**
         * Check wheter the given number is a power of two or not
         * Requires type_t to be an integral type
         * @param value The value to test
         * @return true if value is a power of two, false otherwise
         */
        template<typename type_t>
        requires(is_integral_v<type_t>)
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
    }; // namespace math

} // namespace hud
#endif // HD_INC_CORE_MATH_H