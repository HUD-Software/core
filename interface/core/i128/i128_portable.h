#ifndef HD_INC_CORE_I128_I128_PORTABLE_H
#define HD_INC_CORE_I128_I128_PORTABLE_H
// #include "../assert.h"
#include <math.h> //ldexpf, ld
#include "../limits.h"
#include "../math.h"
#include "../assert.h"
#include <limits>

namespace hud
{
    namespace details::i128
    {

#if defined(HD_LITTLE_ENDIAN)

        struct u128_portable;
        struct i128_portable;

        /** i128 no intrinsic implementation for little endian platform. */
        struct alignas(16) i128_portable
        {
            /** Default constructor. */
            constexpr i128_portable() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_portable(i64 high, u64 low) noexcept
                : low_(low)
                , high_(high)
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
                : low_ {static_cast<u64>(value)}
                , high_ {value < 0 ? ~i64 {0} : 0}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_portable(i64 value) noexcept
                : low_ {static_cast<u64>(value)}
                , high_ {0}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_portable(u64 value) noexcept
                : low_ {value}
                , high_ {0}
            {
            }

            /** Construct a i128 from i128. */
            explicit constexpr i128_portable(u128_portable value) noexcept;

            /** Construct a i128 from f32. */
            i128_portable(f32 value) noexcept;

            /** Construct a i128 from f64. */
            i128_portable(f64 value) noexcept;

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return low_;
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr i64 high() const noexcept
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
                return static_cast<i8>(static_cast<i64>(*this));
            }

            /** Cast to u8. */
            [[nodiscard]] explicit constexpr operator u8() const noexcept
            {
                return static_cast<u8>(low_);
            }

            /** Cast to i16. */
            [[nodiscard]] explicit constexpr operator i16() const noexcept
            {
                return static_cast<i16>(static_cast<i64>(*this));
            }

            /** Cast to u16. */
            [[nodiscard]] explicit constexpr operator u16() const noexcept
            {
                return static_cast<u16>(low_);
            }

            /** Cast to i32. */
            [[nodiscard]] explicit constexpr operator i32() const noexcept
            {
                return static_cast<i32>(static_cast<i64>(*this));
            }

            /** Cast to u32. */
            [[nodiscard]] explicit constexpr operator u32() const noexcept
            {
                return static_cast<u32>(low_);
            }

            /** Cast to i64. */
            [[nodiscard]] explicit constexpr operator i64() const noexcept
            {
                // Casting an unsigned integer to a signed integer of the same
                // width is implementation defined behavior if the source value would not fit
                // in the destination type. We step around it with a roundtrip bitwise not
                // operation to make sure this function remains constexpr. Clang, GCC, and
                // MSVC optimize this to a no-op on x86-64.
                return low_ & (u64 {1} << 63) ? ~static_cast<i64>(~low_) : static_cast<i64>(low_);
            }

            /** Cast to u64. */
            [[nodiscard]] explicit constexpr operator u64() const noexcept
            {
                return static_cast<u64>(low_);
            }

            /** Cast to ansichar. */
            [[nodiscard]] explicit constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(static_cast<i64>(*this));
            }

            /** Cast to wchar. */
            [[nodiscard]] explicit constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(static_cast<i64>(*this));
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
            [[nodiscard]] explicit constexpr operator f32() const noexcept;

            /** Cast to f64. */
            [[nodiscard]] explicit constexpr operator f64() const noexcept;

            /** Checks whether other i128_portable is equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(i128_portable other) const noexcept
            {
                return low_ == other.low_ && high_ == other.high_;
            }

            /** Checks whether other i128_portable is not equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(i128_portable other) const noexcept
            {
                return !(*this == other);
            }

            /** Checks whether other i128_portable is less than this */
            [[nodiscard]] constexpr bool operator<(i128_portable other) const noexcept
            {
                return (high_ == other.high_) ? (low_ < other.low_) : (high_ < other.high_);
            }

            /** Checks whether other i128_portable is less or equal than this */
            [[nodiscard]] constexpr bool operator<=(i128_portable other) const noexcept
            {
                return !(*this > other);
            }

            /** Checks whether other i128_portable is greater than this */
            [[nodiscard]] constexpr bool operator>(i128_portable other) const noexcept
            {
                return (high_ == other.high_) ? (low_ > other.low_) : (high_ > other.high_);
            }

            /** Checks whether other i128_portable is greater or equal than this */
            [[nodiscard]] constexpr bool operator>=(i128_portable other) const noexcept
            {
                return !(*this < other);
            }

            /** Add this to other and return the result. */
            [[nodiscard]] constexpr i128_portable operator+(i128_portable other) const noexcept
            {
                // check for carry
                i128_portable result {high_ + other.high_, low_ + other.low_};
                return (result.low_ < other.low_) ? i128_portable {result.high_ + 1, result.low_} : result;
            }

            /** Add this to other and return this. */
            constexpr i128_portable &operator+=(i128_portable other) noexcept
            {
                *this = *this + other;
                return *this;
            }

            /** Subtract this to other and return the result. */
            [[nodiscard]] constexpr i128_portable operator-(i128_portable other) const noexcept
            {
                // check for carry
                i128_portable result {high_ - other.high_, low_ - other.low_};
                return (low_ < other.low_) ? i128_portable {result.high_ - 1, result.low_} : result;
            }

            /** Subtract this to other and return this. */
            constexpr i128_portable &operator-=(i128_portable other) noexcept
            {
                *this = *this - other;
                return *this;
            }

            /** Multiply this to other and return the result. */
            [[nodiscard]] constexpr i128_portable operator*(i128_portable other) const noexcept;

            /** Multiply this to other and return this. */
            i128_portable &operator*=(i128_portable other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            /** Divide this to other and return the result. */
            [[nodiscard]] i128_portable operator/(i128_portable other) const noexcept;

            /** Multiply this to other and return this. */
            i128_portable &operator/=(i128_portable other) noexcept
            {
                *this = *this / other;
                return *this;
            }

            /** Retrieves the remainder of this divided by other and returns the result. */
            [[nodiscard]] i128_portable operator%(i128_portable other) const noexcept;

            /** Retrieves the remainder of this divided by other, result in this and returns this. */
            i128_portable &operator%=(i128_portable other) noexcept
            {
                *this = *this % other;
                return *this;
            }

            /** Negate this. */
            [[nodiscard]] constexpr i128_portable operator-() const noexcept
            {
                return i128_portable {~high_ + (low_ == 0), ~low_ + 1};
            }

            /** Check if this is zero. */
            [[nodiscard]] constexpr bool operator!() const noexcept
            {
                return !low_ && !high_;
            }

            /** Invert this bits. */
            [[nodiscard]] constexpr i128_portable operator~() const noexcept
            {
                return i128_portable {~high_, ~low_};
            }

            /** Retrieves logical this OR other and return the result.*/
            [[nodiscard]] constexpr i128_portable operator|(i128_portable other) const noexcept
            {
                return i128_portable {high_ | other.high_, low_ | other.low_};
            }

            /** Retrieves logical this AND other and return the result.*/
            [[nodiscard]] constexpr i128_portable operator&(i128_portable other) const noexcept
            {
                return i128_portable {high_ & other.high_, low_ & other.low_};
            }

            /** Retrieves logical this XOR other and return the result.*/
            [[nodiscard]] constexpr i128_portable operator^(i128_portable other) const noexcept
            {
                return i128_portable {high_ ^ other.high_, low_ ^ other.low_};
            }

            /** Logical OR */
            constexpr i128_portable &operator|=(i128_portable other) noexcept
            {
                *this = *this | other;
                return *this;
            }

            /** Logical AND */
            constexpr i128_portable &operator&=(i128_portable other) noexcept
            {
                *this = *this & other;
                return *this;
            }

            /** Logical XOR */
            constexpr i128_portable &operator^=(i128_portable other) noexcept
            {
                *this = *this ^ other;
                return *this;
            }

            /** Post increment this. */
            constexpr i128_portable operator++(i32) noexcept
            {
                i128_portable tmp(*this);
                *this += 1;
                return tmp;
            }

            /** Pre increment this. */
            constexpr i128_portable &operator++() noexcept
            {
                *this += 1;
                return *this;
            }

            /** Post decrement this. */
            constexpr i128_portable operator--(i32) noexcept
            {
                i128_portable tmp(*this);
                *this -= 1;
                return tmp;
            }

            /** Pre decrement this. */
            constexpr i128_portable &operator--() noexcept
            {
                *this -= 1;
                return *this;
            }

            /** Left shift this and return the result */
            constexpr i128_portable operator<<(i32 amount) const noexcept;

            /** Right shift this and return the result */
            constexpr i128_portable operator>>(i32 amount) const noexcept;

            /** Left shift this and return the result */
            constexpr i128_portable &operator<<=(i32 amount) noexcept
            {
                *this = *this << amount;
                return *this;
            }

            /** Right shift this and return the result */
            const i128_portable &operator>>=(i32 amount) noexcept
            {
                *this = *this >> amount;
                return *this;
            }

            /** The low part of the i128. */
            u64 low_;
            /** The high part of the i128. */
            i64 high_;
        };

        static constexpr i128_portable i128_max = i128_portable {i64_max, u64_max};
        static constexpr i128_portable i128_min = i128_portable {i64_min, 0u};

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
            u128_portable(f32 value) noexcept;

            /** Construct a i128 from f64. */
            u128_portable(f64 value) noexcept;

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
            [[nodiscard]] constexpr bool operator==(u128_portable other) const noexcept
            {
                return low_ == other.low_ && high_ == other.high_;
            }

            /** Checks whether other u128_portable is not equal to this */
            [[nodiscard]] constexpr bool operator!=(u128_portable other) const noexcept
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
                return other < *this;
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

            /** Add this to other and return this. */
            constexpr u128_portable &operator+=(u128_portable other) noexcept
            {
                *this = *this + other;
                return *this;
            }

            /** Subtract this to other and return the result. */
            [[nodiscard]] constexpr u128_portable operator-(u128_portable other) const noexcept
            {
                u128_portable result {high_ - other.high_, low_ - other.low_};
                return (low_ < other.low_) ? u128_portable(result.high_ - 1, result.low_) : result;
            }

            /** Subtract this to other and return this. */
            constexpr u128_portable &operator-=(u128_portable other) noexcept
            {
                *this = *this - other;
                return *this;
            }

            /** Multiply this to other and return the result. */
            constexpr u128_portable operator*(u128_portable other) const noexcept
            {
    #if defined(HD_COMPILER_MSVC) && defined(HD_TARGET_X64)
                if (!hud::is_constant_evaluated())
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

            /** Multiply this to other and return this. */
            u128_portable &operator*=(u128_portable other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            /** Divide this to other and return the result. */
            [[nodiscard]] u128_portable operator/(u128_portable other) const noexcept;

            /** Divide this to other and return this. */
            u128_portable &operator/=(u128_portable other) noexcept
            {
                *this = *this / other;
                return *this;
            }

            /** Retrieves the remainder of this divided by other and returns the result. */
            [[nodiscard]] u128_portable operator%(u128_portable other) const noexcept;

            /** Retrieves the remainder of this divided by other, result in this and returns this. */
            u128_portable &operator%=(u128_portable other) noexcept
            {
                *this = *this % other;
                return *this;
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

        static constexpr u128_portable u128_max = u128_portable {u64_max, u64_max};
        static constexpr u128_portable u128_min = u128_portable {u64_min, 0u};

        constexpr i128_portable::i128_portable(u128_portable value) noexcept
            : low_ {value.low()}
            , high_ {static_cast<i64>(value.high())}
        {
        }

        constexpr i128_portable::operator f32() const noexcept
        {
            // We must convert the absolute value and then negate as needed, because
            // floating point types are typically sign-magnitude. Otherwise, the
            // difference between the high and low 64 bits when interpreted as two's
            // complement overwhelms the precision of the mantissa.
            //
            // Also check to make sure we don't negate int128_portable()
            return high_ < 0 && *this != i128_min ? -static_cast<f32>(-*this) : static_cast<f32>(low_) + hud::math::ldexp(static_cast<f32>(high_), 64);
        }

        /** Cast to f64. */
        constexpr i128_portable::operator f64() const noexcept
        {
            // See comment in i128_portable::operator f32() above.
            return high_ < 0 && *this != i128_min ? -static_cast<f64>(-*this) : static_cast<f64>(low_) + hud::math::ldexp(static_cast<f64>(high_), 64);
        }

        constexpr i128_portable i128_portable::operator*(i128_portable other) const noexcept
        {
            u128_portable this_mul_other = u128_portable(*this) * u128_portable(other);
            return {static_cast<i64>(this_mul_other.high_), this_mul_other.low_};
        }

        constexpr i128_portable i128_portable::operator<<(i32 amount) const noexcept
        {
            // i64 shifts of >= 63 are undefined, so we need some special-casing.
            hud::check(amount >= 0 && amount < 127);
            if (amount <= 0)
            {
                return *this;
            }
            else if (amount < 63)
            {
                return i128_portable {
                    (high_ << amount) | static_cast<i64>(low_ >> (64 - amount)),
                    low_ << amount
                };
            }
            else if (amount == 63)
            {
                return i128_portable {((high_ << 32) << 31) | static_cast<i64>(low_ >> 1), (low_ << 32) << 31};
            }
            else if (amount == 127)
            {
                return i128_portable {static_cast<i64>(low_ << 63), 0};
            }
            else if (amount > 127)
            {
                return i128_portable {0, 0};
            }

            // amount >= 64 && amount < 127
            return i128_portable(static_cast<i64>(low_ << (amount - 64)), 0);
        }

        constexpr i128_portable i128_portable::operator>>(i32 amount) const noexcept
        {
            // i64 shifts of >= 63 are undefined, so we need some special-casing.
            hud::check(amount >= 0 && amount < 127);
            if (amount <= 0)
            {
                return *this;
            }
            else if (amount < 63)
            {
                return i128_portable {
                    high_ >> amount,
                    low_ >> amount | static_cast<u64>(high_) << (64 - amount)
                };
            }
            else if (amount == 63)
            {
                return i128_portable((high_ >> 32) >> 31, static_cast<u64>(high_ << 1) | (low_ >> 32) >> 31);
            }
            else if (amount >= 127)
            {
                return i128_portable((high_ >> 32) >> 31, static_cast<u64>((high_ >> 32) >> 31));
            }
            else
            {
                // amount >= 64 && amount < 127
                return i128_portable(
                    (high_ >> 32) >> 31,
                    static_cast<u64>(high_ >> (amount - 64))
                );
            }
        }

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

    } // namespace details::i128

    // static inline constexpr details::i128::i128_impl i128_max = details::i128::i128_impl {i64_max, u64_max};
    // static inline constexpr details::i128::i128_impl i128_min = details::i128::i128_impl {i64_min, 0u};

    // static inline constexpr details::i128::u128_impl u128_max = details::i128::u128_impl {u64_max, u64_max};
    // static inline constexpr details::i128::u128_impl u128_min = details::i128::u128_impl {u64_min, 0u};

    // template<> struct limits<details::i128::i128_impl>
    // {
    //     static constexpr details::i128::i128_impl min {hud::i128_min};
    //     static constexpr details::i128::i128_impl max {hud::i128_max};
    // };

    // template<> struct limits<details::i128::u128_impl>
    // {
    //     static constexpr details::i128::u128_impl min {hud::u128_min};
    //     static constexpr details::i128::u128_impl max {hud::u128_max};
    // };

} // namespace hud

#endif // HD_INC_CORE_I128_I128_PORTABLE_H