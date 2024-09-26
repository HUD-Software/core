#ifndef HD_INC_CORE_I128_I128_PORTABLE_H
#define HD_INC_CORE_I128_I128_PORTABLE_H
#include "assert.h"
#include <math.h> //ldexpf, ld

namespace hud
{
    class i128;
    class u128;

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
#if defined(HD_LITTLE_ENDIAN)
        class u128_portable;

        /** i128 no intrinsic implementation for little endian platform. */
        struct alignas(16) i128_portable
        {
            /** Default constructor. */
            constexpr i128_portable() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_portable(i64 high, u64 low) noexcept
                : low_ {low}
                , high_ {high}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_portable(i32 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? ~i64 {0} : 0}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_portable(u32 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_portable(i64 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? ~i64 {0} : 0}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_portable(u64 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a i128 from f32. */
            i128_portable(f32 value) noexcept
            {
            }

            /** Construct a i128 from f64. */
            i128_portable(f64 value) noexcept
            {
            }

            /** Construct a i128 from u128. */
            explicit constexpr i128_portable(u128_portable value) noexcept;

            /** The low part of the i128. */
            u64 low_;
            /** The high part of the i128. */
            i64 high_;
        };

        /** u128 no intrinsic implementation for little endian platform. */
        struct alignas(16) u128_portable
        {
            /** Default constructor. */
            constexpr u128_portable() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_portable(u64 high, u64 low) noexcept
                : low_ {low}
                , high_ {high}
            {
            }

            /** Construct a u128 from i32. */
            constexpr u128_portable(i32 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? hud::u64_max : 0}
            {
            }

            /** Construct a u128 from u32. */
            constexpr u128_portable(u32 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a u128 from i64. */
            constexpr u128_portable(i64 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? hud::u64_max : 0}
            {
            }

            /** Construct a u128 from u64. */
            constexpr u128_portable(u64 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a u128 from i128. */
            explicit constexpr u128_portable(i128_portable value) noexcept
                : low_ {value.low_}
                , high_ {static_cast<u64>(value.high_)}
            {
            }

            /** Construct a i128 from f32. */
            u128_portable(f32 value) noexcept
            {
                u128_from_f32(value, high_, low_);
            }

            /** Construct a i128 from f64. */
            u128_portable(f64 value) noexcept
            {
                u128_from_f64(value, high_, low_);
            }

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return low_;
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr u64 high() const noexcept
            {
                return high_;
            }

            /** Cast to bool. */
            [[nodiscard]] explicit constexpr operator bool() const noexcept
            {
                return low_ || high_;
            }

            /** Cast to i8. */
            [[nodiscard]] explicit constexpr operator i8() const noexcept
            {
                return static_cast<i8>(low_);
            }

            /** Cast to u8. */
            [[nodiscard]] explicit constexpr operator u8() const noexcept
            {
                return static_cast<u8>(low_);
            }

            /** Cast to i16. */
            [[nodiscard]] explicit constexpr operator i16() const noexcept
            {
                return static_cast<i16>(low_);
            }

            /** Cast to u16. */
            [[nodiscard]] explicit constexpr operator u16() const noexcept
            {
                return static_cast<u16>(low_);
            }

            /** Cast to i32. */
            [[nodiscard]] explicit constexpr operator i32() const noexcept
            {
                return static_cast<i32>(low_);
            }

            /** Cast to u32. */
            [[nodiscard]] explicit constexpr operator u32() const noexcept
            {
                return static_cast<u32>(low_);
            }

            /** Cast to i64. */
            [[nodiscard]] explicit constexpr operator i64() const noexcept
            {
                return static_cast<i64>(low_);
            }

            /** Cast to u64. */
            [[nodiscard]] explicit constexpr operator u64() const noexcept
            {
                return low_;
            }

            /** Cast to ansichar. */
            [[nodiscard]] explicit constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(low_);
            }

            /** Cast to wchar. */
            [[nodiscard]] explicit constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(low_);
            }

            /** Cast to char16. */
            [[nodiscard]] explicit constexpr operator char16() const noexcept
            {
                return static_cast<char16>(low_);
            }

            /** Cast to char32. */
            [[nodiscard]] explicit constexpr operator char32() const noexcept
            {
                return static_cast<char32>(low_);
            }

            /** Cast to f32. */
            [[nodiscard]] explicit constexpr operator f32() const noexcept
            {
                return static_cast<f32>(low_) + hud::math::ldexp(static_cast<f32>(high_), 64);
            }

            /** Cast to f64. */
            [[nodiscard]] explicit constexpr operator f64() const noexcept
            {
                return static_cast<f64>(low_) + hud::math::ldexp(static_cast<f64>(high_), 64);
            }

            /** Checks whether other u128_portable is equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(u128_portable other) const noexcept
            {
                return low_ == other.low_ && high_ == other.high_;
            }

            /** Checks whether other u128_portable is not equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(u128_portable other) const noexcept
            {
                return !(*this == other);
            }

            /** Checks whether other u128_portable is less than this */
            [[nodiscard]] constexpr bool operator<(u128_portable other) const noexcept
            {
                return high_ == other.high_ ? low_ < other.low_ : high_ < other.high_;
            }

            /** Checks whether other u128_portable is less or equal than this */
            [[nodiscard]] constexpr bool operator<=(u128_portable other) const noexcept
            {
                return !(other < *this);
            }

            /** Checks whether other u128_portable is greater than this */
            [[nodiscard]] constexpr bool operator>(u128_portable other) const noexcept
            {
                return *this < other;
            }

            /** Checks whether other u128_portable is greater or equal than this */
            [[nodiscard]] constexpr bool operator>=(u128_portable other) const noexcept
            {
                return !(*this < other);
            }

            /** Add this to other and return the result. */
            [[nodiscard]] constexpr u128_portable operator+(u128_portable other) const noexcept
            {
                u128_portable result {high_ + other.high_, low_ + other.low_};
                return (result.low_ < other.low_) ? u128_portable {result.high_ + 1, result.low_} : result;
            }

            /** Subtract this to other and return the result. */
            [[nodiscard]] constexpr u128_portable operator-(u128_portable other) const noexcept
            {
                u128_portable result {high_ - other.high_, low_ - other.low_};
                return (low_ < other.low_) ? u128_portable(result.high_ - 1, result.low_) : result;
            }

            /** Multiply this to other and return the result. */
            [[nodiscard]] constexpr u128_portable operator*(u128_portable other) const noexcept
            {
    #if defined(HD_COMPILER_MSVC) && defined(HD_TARGET_X64)
                if (hud::is_constant_evaluated())
                {
                    u64 carry;
                    u64 low = _umul128(low_, other.low_, &carry);
                    return u128_portable(low_ * other.high_ + high_ * other.low_ + carry, low);
                }
                else
                {
                    u64 a32 = low_ >> 32;
                    u64 a00 = low_ & 0xffffffff;
                    u64 b32 = other.low_ >> 32;
                    u64 b00 = other.low_ & 0xffffffff;
                    u128_portable result = u128_portable(high_ * other.low_ + low_ * other.high_ + a32 * b32, a00 * b00);
                    result += u128_portable(a32 * b00) << 32;
                    result += u128_portable(a00 * b32) << 32;
                    return result;
                }
    #else
                u64 a32 = low_ >> 32;
                u64 a00 = low_ & 0xffffffff;
                u64 b32 = other.low_ >> 32;
                u64 b00 = other.low_ & 0xffffffff;
                u128_portable result = u128_portable(high_ * other.low_ + low_ * other.high_ + a32 * b32, a00 * b00);
                result += u128_portable(a32 * b00) << 32;
                result += u128_portable(a00 * b32) << 32;
                return result;
    #endif
            }

            /** Divide this to other and return the result. */
            [[nodiscard]] constexpr u128_portable operator/(u128_portable other) const noexcept
            {
                return intrinsic_value_ / other.intrinsic_value_;
            }

            /** Retrieves the remainder of this divided by other and returns the result. */
            [[nodiscard]] constexpr u128_portable operator%(u128_portable other) const noexcept
            {
                return intrinsic_value_ % other.intrinsic_value_;
            }

            /** Negate this. */
            [[nodiscard]] constexpr u128_portable operator-() const noexcept
            {
                return u128_portable {
                    ~high_ + static_cast<u32>(low_ == 0),
                    ~low_ + 1
                };
            }

            /** Check if this is zero. */
            [[nodiscard]] constexpr bool operator!() const noexcept
            {
                return !high_ && !low_;
            }

            /** Invert this bits. */
            [[nodiscard]] constexpr u128_portable operator~() const noexcept
            {
                return u128_portable {
                    ~high_,
                    ~low_
                };
            }

            /** Retrieves logical this OR other and return the result.*/
            [[nodiscard]] constexpr u128_portable operator|(u128_portable other) const noexcept
            {
                return u128_portable {
                    high_ | other.high_,
                    low_ | other.low_
                };
            }

            /** Retrieves logical this AND other and return the result.*/
            [[nodiscard]] constexpr u128_portable operator&(u128_portable other) const noexcept
            {
                return u128_portable {
                    high_ & other.high_,
                    low_ & other.low_
                };
            }

            /** Retrieves logical this XOR other and return the result.*/
            [[nodiscard]] constexpr u128_portable operator^(u128_portable other) const noexcept
            {
                return u128_portable {
                    high_ ^ other.high_,
                    low_ ^ other.low_
                };
            }

            /** Logical OR */
            constexpr u128_portable &operator|=(u128_portable other) noexcept
            {
                *this = *this | other;
                return *this;
            }

            /** Logical AND */
            constexpr u128_portable &operator&=(u128_portable other) noexcept
            {
                *this = *this & other;
                return *this;
            }

            /** Logical XOR */
            constexpr u128_portable &operator^=(u128_portable other) noexcept
            {
                *this = *this ^ other;
                return *this;
            }

            /** Post increment this. */
            constexpr u128_portable operator++(i32) noexcept
            {
                u128_portable tmp(*this);
                *this += 1;
                return tmp;
            }

            /** Pre increment this. */
            constexpr u128_portable &operator++() noexcept
            {
                *this += 1;
                return *this;
            }

            /** Post decrement this. */
            constexpr u128_portable operator--(i32) noexcept
            {
                u128_portable tmp(*this);
                *this -= 1;
                return tmp;
            }

            /** Pre decrement this. */
            constexpr u128_portable &operator--() noexcept
            {
                *this -= 1;
                return *this;
            }

            /** Left shift this and return the result */
            constexpr u128_portable operator<<(i32 amount) const noexcept
            {
                // uint64_t shifts of >= 64 are undefined, so we will need some
                // special-casing.
                return amount >= 64 ? u128_portable(low_ << (amount - 64), 0) :
                       amount == 0  ? *this :
                                      u128_portable((high_ << amount) | (low_ >> (64 - amount)), low_ << amount);
            }

            /** Right shift this and return the result */
            const u128_portable operator>>(i32 amount) const noexcept
            {
                // uint64_t shifts of >= 64 are undefined, so we will need some
                // special-casing.
                return amount >= 64 ? u128_portable(0, high_ >> (amount - 64)) :
                       amount == 0  ? *this :
                                      u128_portable(high_ >> amount, (low_ >> amount) | (high_ << (64 - amount)));
            }

            /** Left shift this and return the result */
            constexpr u128_portable &operator<<=(i32 amount) noexcept
            {
                *this = *this << amount;
                return *this;
            }

            /** Right shift this and return the result */
            const u128_portable &operator>>=(i32 amount) noexcept
            {
                *this = *this >> amount;
                return *this;
            }

            /** The low part of the u128. */
            u64 low_;
            /** The high part of the u128. */
            u64 high_;
        };

#elif defined(HD_BIG_ENDIAN)
        /** i128 no intrinsic implementation for big endian platform. */
        struct alignas(16) i128_portable
        {
            /** Default constructor. */
            constexpr i128_portable() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_portable(i64 high, u64 low) noexcept
                : high_ {high}
                , low_ {low}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_portable(i32 value) noexcept
                : high_ {value < 0 ? ~i64 {0} : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_portable(u32 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_portable(i64 value) noexcept
                : high_ {value < 0 ? ~i64 {0} : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_portable(u64 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a i128 from u128. */
            explicit constexpr i128_portable(hud::u128 value) noexcept;

            /** Construct a i128 from f32. */
            i128_portable(f32 value) noexcept
            {
            }

            /** Construct a i128 from f64. */
            i128_portable(f64 value) noexcept
            {
            }

            /** The high part of the i128. */
            i64 high_;
            /** The low part of the i128. */
            u64 low_;
        };

        /** u128 no intrinsic implementation for big endian platform. */
        struct alignas(16) u128_big
        {
            /** Default constructor. */
            constexpr u128_big() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_big(u64 high, u64 low) noexcept
                : high_ {high}
                , low_ {low}
            {
            }

            /** Construct a u128 from i32. */
            constexpr u128_big(i32 value) noexcept
                : high_ {value < 0 ? hud::u64_max : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a u128 from u32. */
            constexpr u128_big(u32 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a u128 from i64. */
            constexpr u128_big(i64 value) noexcept
                : high_ {value < 0 ? hud::u64_max : 0}
                , low_ {static_cast<u64>(value)}
            {
            }

            /** Construct a u128 from u64. */
            constexpr u128_big(u64 value) noexcept
                : high_ {0}
                , low_ {value}
            {
            }

            /** Construct a u128 from i128. */
            explicit constexpr u128_big(hud::i128 value) noexcept;

            /** Construct a i128 from f32. */
            u128_big(f32 value) noexcept
            {
                u128_from_f32(value, high_, low_);
            }

            /** Construct a i128 from f64. */
            u128_big(f64 value) noexcept
            {
                u128_from_f64(value, high_, low_);
            }

            /** The high part of the u128. */
            u64 high_;
            /** The low part of the u128. */
            u64 low_;
        };

#endif
        using i128_impl = details::i128::i128_portable;
        using u128_impl = details::i128::u128_portable;

        // using u128_impl = hud::conditional_t<hud::compilation::is_endianness(endianness_e::little), details::i128::u128_portable, details::i128::u128_big>;
    } // namespace details::i128
} // namespace hud

#endif // HD_INC_CORE_I128_I128_PORTABLE_H