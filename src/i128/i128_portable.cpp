#if !HD_INTRINSIC_INT128_SUPPORTED
    #include <core/minimal.h>
    #include <core/assert.h>
    #include <core/i128.h>
    #include <core/math.h>

    #include <limits>

namespace hud
{
    namespace details::i128
    {

        static inline void u128_from_f32(f32 value, u64 &high, u64 &low) noexcept
        {
            // Check value is not NaN or infinite
            hud::check(hud::math::is_finite(value));
            // Ensure we are positive
            hud::check(value > -1);
            if (value >= hud::math::ldexp(static_cast<f32>(1), 64))
            {
                high = static_cast<u64>(hud::math::ldexp(value, -64));
                low = static_cast<u64>(value - hud::math::ldexp(static_cast<f32>(high), 64));
            }
            else
            {
                high = 0;
                low = static_cast<u64>(value);
            }
        }

        static inline void u128_from_f64(f64 value, u64 &high, u64 &low) noexcept
        {
            // Check value is not NaN or infinite
            hud::check(hud::math::is_finite(value));
            // Ensure we are positive
            hud::check(value > -1);
            // Check value is lower than 2^128
            hud::check(value < hud::math::ldexp(static_cast<f64>(1), 128));

            // If value is greater than value*(2^64)
            if (value >= hud::math::ldexp(static_cast<f64>(1), 64))
            {
                high = static_cast<u64>(hud::math::ldexp(value, -64));
                low = static_cast<u64>(value - hud::math::ldexp(static_cast<f64>(high), 64));
            }
            else
            {
                high = 0;
                low = static_cast<u64>(value);
            }
        }

        // Returns the 0-based position of the last set bit (i.e., most significant bit)
        // in the given u128_portable. The argument is not 0.
        //
        // For example:
        //   Given: 5 (decimal) == 101 (binary)
        //   Returns: 2
        static constexpr i32 fls128(u128_portable n)
        {
            if (u64 hi = n.high_)
            {
                // HD_ASSUME(hi != 0);
                return 127 - hud::math::count_leading_zero(hi);
            }
            const u64 low = n.low_;
            // HD_ASSUME(low != 0);
            return 63 - hud::math::count_leading_zero(low);
        }

        // Long division/modulo for uint128 implemented using the shift-subtract
        // division algorithm adapted from:
        // https://stackoverflow.com/questions/5386377/division-without-using
        static constexpr void div_mod_impl(u128_portable dividend, u128_portable divisor, u128_portable *quotient_ret, u128_portable *remainder_ret)
        {
            hud::check(divisor != 0);

            if (divisor > dividend)
            {
                *quotient_ret = 0;
                *remainder_ret = dividend;
                return;
            }

            if (divisor == dividend)
            {
                *quotient_ret = 1;
                *remainder_ret = 0;
                return;
            }

            u128_portable denominator = divisor;
            u128_portable quotient = 0;

            // Left aligns the MSB of the denominator and the dividend.
            const i32 shift = fls128(dividend) - fls128(denominator);
            denominator <<= shift;

            // Uses shift-subtract algorithm to divide dividend by denominator. The
            // remainder will be left in dividend.
            for (i32 i = 0; i <= shift; ++i)
            {
                quotient <<= 1;
                if (dividend >= denominator)
                {
                    dividend -= denominator;
                    quotient |= 1;
                }
                denominator >>= 1;
            }

            *quotient_ret = quotient;
            *remainder_ret = dividend;
        }

        static void i128_from_f32(f32 value, i64 &high, u64 &low) noexcept
        {
            // Check value is not NaN or infinite
            hud::check(hud::math::is_finite(value));
            // Ensure we fit in a i128 and value is between [-2^127, 2^127]
            hud::check((std::numeric_limits<f32>::max_exponent <= 127) || ((value >= -hud::math::ldexp(f32 {1}, 127)) && value < hud::math::ldexp(f32 {1}, 127)));
            // // Ensure value is between [-2^127, 2^127]
            // hud::check(value >= -hud::math::ldexp(f32 {1}, 127));
            // hud::check(value < hud::math::ldexp(f32 {1}, 127));

            // We must convert the absolute value and then negate as needed, because
            // floating point types are typically sign-magnitude. Otherwise, the
            // difference between the high and low 64 bits when interpreted as two's
            // complement overwhelms the precision of the mantissa.
            u128_portable result = value < 0 ? -u128_portable {-value} : u128_portable {value};
            high = result.high_;
            low = result.low_;
        }

        static void i128_from_f64(f64 value, i64 &high, u64 &low) noexcept
        {
            // Check value is not NaN or infinite
            hud::check(hud::math::is_finite(value));
            // Ensure we fit in a i128  // Ensure value is between [-2^127, 2^127]
            hud::check((std::numeric_limits<f64>::max_exponent <= 127) || ((value >= -hud::math::ldexp(f64 {1}, 127)) && value < hud::math::ldexp(f64 {1}, 127)));
            // hud::check(std::numeric_limits<f64>::max_exponent <= 127);
            // // Ensure value is between [-2^127, 2^127]
            // hud::check(value >= -hud::math::ldexp(f64 {1}, 127));
            // hud::check(value < hud::math::ldexp(f64 {1}, 127));

            // We must convert the absolute value and then negate as needed, because
            // floating point types are typically sign-magnitude. Otherwise, the
            // difference between the high and low 64 bits when interpreted as two's
            // complement overwhelms the precision of the mantissa.
            u128_portable result = value < 0 ? -u128_portable {-value} : u128_portable {value};
            high = result.high_;
            low = result.low_;
        }

    #if defined(HD_LITTLE_ENDIAN)

        i128_portable::i128_portable(f32 value) noexcept
        {
            i128_from_f32(value, high_, low_);
        }

        i128_portable::i128_portable(f64 value) noexcept
        {
            i128_from_f64(value, high_, low_);
        }

        i128_portable i128_portable::operator/(i128_portable other) const noexcept
        {
            hud::check(*this != hud::i128_min || other != -1); // UB on two's complement.

            u128_portable quotient {0};
            u128_portable remainder {0};
            // Cast to uint128 before possibly negating because -Int128Min() is undefined.
            u128_portable u_this = high_ < 0 ? -u128_portable(*this) : u128_portable(*this);
            u128_portable u_other = other.high_ < 0 ? -u128_portable(other) : u128_portable(other);
            div_mod_impl(u_this, u_other, &quotient, &remainder);
            if ((high_ < 0) != (other.high_ < 0))
                quotient = -quotient;
            return i128_portable(static_cast<i64>(quotient.high_), quotient.low_);
        }

        i128_portable i128_portable::operator%(i128_portable other) const noexcept
        {
            hud::check(*this != hud::i128_min || other != -1); // UB on two's complement.

            u128_portable quotient {0};
            u128_portable remainder {0};
            // Cast to uint128 before possibly negating because -Int128Min() is undefined.
            u128_portable u_this = high_ < 0 ? -u128_portable(*this) : u128_portable(*this);
            u128_portable u_other = other.high_ < 0 ? -u128_portable(other) : u128_portable(other);
            div_mod_impl(u_this, u_other, &quotient, &remainder);
            if (high_ < 0)
                remainder = -remainder;
            return i128_portable(static_cast<i64>(remainder.high_), remainder.low_);
        }

        u128_portable::u128_portable(f32 value) noexcept
        {
            u128_from_f32(value, high_, low_);
        }

        u128_portable::u128_portable(f64 value) noexcept
        {
            u128_from_f64(value, high_, low_);
        }

        // /** Cast to f32. */
        // u128_portable::operator f32() const noexcept
        // {
        //     return static_cast<f32>(low_) + hud::math::ldexp(static_cast<f32>(high_), 64);
        // }

        // /** Cast to f64. */
        // u128_portable::operator f64() const noexcept
        // {
        //     return static_cast<f64>(low_) + hud::math::ldexp(static_cast<f64>(high_), 64);
        // }

        u128_portable u128_portable::operator/(u128_portable other) const noexcept
        {
            u128_portable quotient {0};
            u128_portable remainder {0};
            div_mod_impl(*this, other, &quotient, &remainder);
            return quotient;
        }

        u128_portable u128_portable::operator%(u128_portable other) const noexcept
        {
            u128_portable quotient {0};
            u128_portable remainder {0};
            div_mod_impl(*this, other, &quotient, &remainder);
            return remainder;
        }

    #elif defined(HD_BIG_ENDIAN)

    #endif
    } // namespace details::i128
} // namespace hud

#endif // HD_INTRINSIC_INT128_SUPPORTED