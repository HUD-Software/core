#ifndef HD_INC_CORE_OS_COMMON_MATH_H
#define HD_INC_CORE_OS_COMMON_MATH_H
#include "../traits/is_integral.h"
#include "../traits/is_floating_point.h"
#include "../traits/is_signed.h"
#include "../traits/is_unsigned.h"
#include "../traits/is_convertible.h"
#include "../templates/bit_cast.h"
#include <cmath>
#include <limits>
#include "../traits/is_constant_evaluated.h"

namespace hud::os::common
{
    struct math
    {
        /** Compute the natual logarithm of value. */
        template<typename type_t>
        [[nodiscard]] static type_t log(const type_t value) noexcept
        {
            return ::log(value);
        }

        /** Compute the natual logarithm of value. */
        [[nodiscard]] static f32 log(const f32 value) noexcept
        {
            return ::logf(value);
        }

        /** Compute the square root of value. */
        template<typename type_t>
        [[nodiscard]] static type_t sqrt(const type_t value) noexcept
        {
            return ::sqrt(value);
        }

        /** Compute the square root of value. */
        [[nodiscard]] static f32 sqrt(const f32 value) noexcept
        {
            return ::sqrtf(value);
        }

        /**
         * Check wheter the given number is a power of two or not
         * Requires type_t to be an integral type
         * @param value The value to test
         * @return true if value is a power of two, false otherwise
         */
        template<typename type_t>
        [[nodiscard]] static constexpr bool is_power_of_two(const type_t value) noexcept
        requires(hud::is_integral_v<type_t>)

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
         * Computes the base 2 logarithm for an integer value.
         * The result is rounded down to the nearest integer.
         * @param value		The value to compute the log of
         * @return			Log2 of value. 0 if value is 0.
         */
        static constexpr u32 floor_log2(u32 value)
        {
            u32 pos = 0;
            if (value >= 1 << 16)
            {
                value >>= 16;
                pos += 16;
            }
            if (value >= 1 << 8)
            {
                value >>= 8;
                pos += 8;
            }
            if (value >= 1 << 4)
            {
                value >>= 4;
                pos += 4;
            }
            if (value >= 1 << 2)
            {
                value >>= 2;
                pos += 2;
            }
            if (value >= 1 << 1)
            {
                pos += 1;
            }
            return pos;
        }

        /**
         * Computes the base 2 logarithm for an integer value.
         * The result is rounded down to the nearest integer.
         * @param value		The value to compute the log of
         * @return			Log2 of value. 0 if value is 0.
         */
        static constexpr u64 floor_log2(u64 value)
        {
            u64 pos = 0;
            if (value >= 1ull << 32)
            {
                value >>= 32;
                pos += 32;
            }
            if (value >= 1ull << 16)
            {
                value >>= 16;
                pos += 16;
            }
            if (value >= 1ull << 8)
            {
                value >>= 8;
                pos += 8;
            }
            if (value >= 1ull << 4)
            {
                value >>= 4;
                pos += 4;
            }
            if (value >= 1ull << 2)
            {
                value >>= 2;
                pos += 2;
            }
            if (value >= 1ull << 1)
            {
                pos += 1;
            }
            return pos;
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 count_leading_zero(u8 value) noexcept
        {
            if (value == 0)
                return 8;

#if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_GCC)
            return u8(__builtin_clz((u32(value) << 1) | 1) - 23);
#else
            return 7 - floor_log2(u32(value));
#endif
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 count_leading_zero(u32 value) noexcept
        {
            if (value == 0)
                return 32;

#if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_GCC)
            return __builtin_clz(value);
#elif defined(HD_COMPILER_MSVC)
            u32 result = 0;
            if (_BitScanReverse((unsigned long *)&result, value))
            {
                return 31 - result;
            }
            return 32;
#else
            return 31 - floor_log2(u32(value));
#endif
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 count_leading_zero(u64 value) noexcept
        {
            if (value == 0)
                return 64;

#if defined(HD_COMPILER_CLANG) || defined(HD_COMPILER_CLANG_CL) || defined(HD_COMPILER_GCC)
            return __builtin_clzll(value);
#elif defined(HD_COMPILER_MSVC)
    #if defined(HD_TARGET_X64)
            u32 result = 0;
            if (_BitScanReverse64((unsigned long *)&result, value))
            {
                return 63 - result;
            }
            return 64;
    #else
            u32 result = 0;
            if ((value >> 32) && _BitScanReverse((unsigned long *)&result, static_cast<u32>(value >> 32)))
            {
                return 31 - result;
            }
            if (_BitScanReverse((unsigned long *)&result, static_cast<u32>(value)))
            {
                return 63 - result;
            }
            return 64;
    #endif
#else
            return 63 - floor_log2(value);
#endif
        }

        enum class fpclassify_e
        {
            subnormal = FP_SUBNORMAL,
            zero = FP_ZERO,
            nan = FP_NAN,
            infinite = FP_INFINITE,
            normal = FP_NORMAL
        };

        /**
         * Categorizes floating point value num into the following categories: zero, subnormal, normal, infinite, NAN.
         * @param value The value to categorize.
         * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO */
        [[nodiscard]] static constexpr fpclassify_e fpclassify(f64 value) noexcept
        {
            // FROM musl __fpclassify.c
            union
            {
                f64 f;
                u64 i;
            } u = {value};

            int e = u.i >> 52 & 0x7ff;
            if (!e)
                return u.i << 1 ? fpclassify_e::subnormal : fpclassify_e::zero;
            if (e == 0x7ff)
                return u.i << 12 ? fpclassify_e::nan : fpclassify_e::infinite;
            return fpclassify_e::normal;
        }

        /**
         * Categorizes floating point value num into the following categories: zero, subnormal, normal, infinite, NAN.
         * @param value The value to categorize.
         * @return FP_INFINITE, FP_NAN, FP_NORMAL, FP_SUBNORMAL, FP_ZERO */
        [[nodiscard]] static constexpr fpclassify_e fpclassify(f32 value) noexcept
        {
            // FROM musl __fpclassifyf.c
            union
            {
                f32 f;
                u32 i;
            } u = {value};

            int e = u.i >> 23 & 0xff;
            if (!e)
                return u.i << 1 ? fpclassify_e::subnormal : fpclassify_e::zero;
            if (e == 0xff)
                return u.i << 9 ? fpclassify_e::nan : fpclassify_e::infinite;
            return fpclassify_e::normal;
        }

        /**
         * Check if the given floating point number has finite value (not infinite or NaN)
         * @param value The value to test
         * @return true if value if finite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_finite(const float value) noexcept
        {
            return (hud::bit_cast<u32>(value) & 0x7F800000U) != 0x7F800000U;
        }

        /**
         * Check if the given floating point number has finite value (not infinite or NaN)
         * @param value The value to test
         * @return true if value if finite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_finite(const double value) noexcept
        {
            return (hud::bit_cast<u64>(value) & 0x7FF0000000000000ULL) != 0x7FF0000000000000ULL;
        }

        /** Check if the given floating point number is a NaN value
         * @param value The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_nan(const float value) noexcept
        {
            return (hud::bit_cast<u32>(value) & 0x7FFFFFFFU) > 0x7F800000U;
        }

        /** Check if the given floating point number is a normal value
         * @param value The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static constexpr bool is_nan(const double value) noexcept
        {
            return (hud::bit_cast<u64>(value) & 0x7FFFFFFFFFFFFFFFULL) > 0x7FF0000000000000ULL;
        }

        /** Check if the given floating point number is a normal value
         * @param value The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static bool is_normal(const float value) noexcept
        {
            return fpclassify(value) == fpclassify_e::normal;
        }

        /** Check if the given floating point number is a normal value
         * @param value The value to test
         * @return true if value is infinite, false otherwise
         */
        [[nodiscard]] static bool is_normal(const double value) noexcept
        {
            return fpclassify(value) == fpclassify_e::normal;
        }

        // template<typename type_t>
        // [[nodiscard]] static constexpr type_t is_inf(type_t value) noexcept
        // {
        //     if (hud::is_constant_evaluated())
        //     {
        //         if constexpr (std::numeric_limits<type_t>::is_signed)
        //         {
        //             return value == std::numeric_limits<type_t>::infinity() || -value == std::numeric_limits<type_t>::infinity();
        //         }
        //         else
        //         {
        //             return value == std::numeric_limits<type_t>::infinity();
        //         }
        //     }
        //     else
        //     {
        //         if constexpr (!std::is_integral_v<type_t>)
        //         {
        //             return std::isinf(value);
        //         }
        //         else
        //         {
        //             return std::isinf(static_cast<f64>(value));
        //         }
        //     }
        // }

        // template<typename type_t>
        // [[nodiscard]] static constexpr type_t is_nan(type_t value) noexcept
        // {
        //     // Implementation from boost https://live.boost.org/doc/libs/1_85_0/boost/math/ccmath/isnan.hpp
        //     if (hud::is_constant_evaluated())
        //     {
        //         return value != value;
        //     }
        //     else
        //     {
        //         if constexpr (!hud::is_integral_v<type_t>)
        //         {
        //             return std::isnan(value);
        //         }
        //         else
        //         {
        //             return std::isnan(static_cast<f64>(value));
        //         }
        //     }
        // }

        template<typename type_t>
        [[nodiscard]] static constexpr type_t abs(type_t value) noexcept
        {
            return (value < (type_t)0) ? -value : value;
        }

        /** Returns 1, 0, or -1 depending on relation of value to 0 */
        template<class type_t>
        static constexpr type_t Sign(const type_t value)
        {
            return (value > (type_t)0) ? (type_t)1 : ((value < (type_t)0) ? (type_t)-1 : (type_t)0);
        }

        /** Returns higher value in a generic way */
        template<class type_t>
        static constexpr type_t Max(const type_t a, const type_t b)
        {
            return (b < a) ? a : b;
        }

        /** Returns lower value in a generic way */
        template<class type_t>
        static constexpr type_t Min(const type_t a, const type_t b)
        {
            return (a < b) ? a : b;
        }

        /**
         * Multiplies a floating point value by the number 2 raised to the exp power.
         * @tparam type_t The integral type
         * @param value The value to compute
         * @param exp The exponent
         * @return value*(2^exp)
         */
        template<typename type_t>
        [[nodiscard]] static constexpr type_t ldexp(type_t value, i32 exp) noexcept
        requires(hud::is_floating_point_v<type_t>)
        {
            // Implementation from boost https://live.boost.org/doc/libs/1_85_0/boost/math/ccmath/ldexp.hpp
            if (hud::is_constant_evaluated())
            {
                return math::abs(value) == type_t {0.f} ? value :
                       !math::is_finite(value)          ? value :
                       math::is_nan(value)              ? value :
                                                          [value, exp]() mutable
                {
                    while (exp > 0)
                    {
                        value *= 2;
                        --exp;
                    }
                    while (exp < 0)
                    {
                        value /= 2;
                        ++exp;
                    }

                    return value;
                }();
            }
            else
            {
                return std::ldexp(value, exp);
            }
        }
    };
} // namespace hud::os::common
#endif // HD_INC_CORE_OS_COMMON_MATH_H