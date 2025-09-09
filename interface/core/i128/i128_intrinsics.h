#ifndef HD_INC_CORE_I128_I128_INTRINSICS_H
#define HD_INC_CORE_I128_I128_INTRINSICS_H
#include "../assert.h"
#include "../math.h"
#if !defined(HD_INTRINSIC_INT128_SUPPORTED)
    #error i128_intrinsics.h is included but HD_INTRINSIC_INT128_SUPPORTED is not set
#endif

namespace hud
{
    namespace details::i128
    {
        struct u128_intrinsics;

        /**
         * This function converts an unsigned 128-bit integer to a signed 128-bit integer
         * while preserving the underlying binary representation. If the 128th bit (bit 127)
         * is set, it indicates that the value cannot fit within a signed __int128, which
         * uses 127 bits for the value and 1 bit for the sign. In such cases, the value is
         * treated as negative by using a bitwise inversion to maintain two's complement behavior.
         * Otherwise, a direct conversion is used. This ensures that the function remains
         * constexpr and avoids undefined behavior.
         * Modern compilers like Clang and GCC optimize this operation to a no-op on x86-64 architectures.
         */
        [[nodiscard]] constexpr __int128 bit_cast_to_signed_int128(unsigned __int128 value) noexcept
        {
            return value & (static_cast<unsigned __int128>(1) << 127) ? ~static_cast<__int128>(~value) : static_cast<__int128>(value);
        }

        /**
         * This function converts an unsigned 64-bit integer to a signed 64-bit integer
         * while preserving the underlying binary representation. If the sign bit (bit 63)
         * is set, it indicates that the value cannot fit within a signed i64, which uses
         * 63 bits for the value and 1 bit for the sign. In such cases, the value is treated
         * as negative by using a bitwise inversion to maintain two's complement behavior.
         * Otherwise, a direct conversion is used. This approach ensures the function remains
         * constexpr and avoids undefined behavior.
         * Compilers like Clang, GCC, and MSVC optimize this operation to a no-op on x86-64 architectures.
         */
        [[nodiscard]] constexpr i64 bit_cast_to_signed_i64(u64 value) noexcept
        {
            return value & (u64 {1} << 63) ? ~static_cast<i64>(~value) : static_cast<i64>(value);
        }

        struct alignas(16) i128_intrinsics
        {
            /** Default constructor. */
            constexpr i128_intrinsics() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_intrinsics(i64 high, u64 low) noexcept
                : intrinsic_value_ {bit_cast_to_signed_int128(static_cast<unsigned __int128>(high) << 64 | low)}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_intrinsics(i32 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_intrinsics(u32 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_intrinsics(i64 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_intrinsics(u64 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr i128_intrinsics(__int128 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr i128_intrinsics(unsigned __int128 value) noexcept
                : intrinsic_value_ {static_cast<__int128>(value)}
            {
            }

            /** Construct a i128 from i128. */
            explicit constexpr i128_intrinsics(u128_intrinsics value) noexcept;

            /** Construct a i128 from f32. */
            constexpr i128_intrinsics(f32 value) noexcept
                : intrinsic_value_ {static_cast<__int128>(value)}
            {
            }

            /** Construct a i128 from f64. */
            constexpr i128_intrinsics(f64 value) noexcept
                : intrinsic_value_ {static_cast<__int128>(value)}
            {
            }

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return static_cast<u64>(intrinsic_value_ & ~u64 {0});
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr i64 high() const noexcept
            {
                return bit_cast_to_signed_i64(static_cast<u64>(static_cast<unsigned __int128>(intrinsic_value_) >> 64));
            }

            /** Cast to bool. */
            [[nodiscard]] explicit constexpr operator bool() const noexcept
            {
                return intrinsic_value_ != 0u;
            }

            /** Cast to i8. */
            [[nodiscard]] explicit constexpr operator i8() const noexcept
            {
                return static_cast<i8>(intrinsic_value_);
            }

            /** Cast to u8. */
            [[nodiscard]] explicit constexpr operator u8() const noexcept
            {
                return static_cast<u8>(intrinsic_value_);
            }

            /** Cast to i16. */
            [[nodiscard]] explicit constexpr operator i16() const noexcept
            {
                return static_cast<i16>(intrinsic_value_);
            }

            /** Cast to u16. */
            [[nodiscard]] explicit constexpr operator u16() const noexcept
            {
                return static_cast<u16>(intrinsic_value_);
            }

            /** Cast to i32. */
            [[nodiscard]] explicit constexpr operator i32() const noexcept
            {
                return static_cast<i32>(intrinsic_value_);
            }

            /** Cast to u32. */
            [[nodiscard]] explicit constexpr operator u32() const noexcept
            {
                return static_cast<u32>(intrinsic_value_);
            }

            /** Cast to i64. */
            [[nodiscard]] explicit constexpr operator i64() const noexcept
            {
                return static_cast<i64>(intrinsic_value_);
            }

            /** Cast to u64. */
            [[nodiscard]] explicit constexpr operator u64() const noexcept
            {
                return intrinsic_value_;
            }

            /** Cast to ansichar. */
            [[nodiscard]] explicit constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(intrinsic_value_);
            }

            /** Cast to wchar. */
            [[nodiscard]] explicit constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(intrinsic_value_);
            }

            /** Cast to char16. */
            [[nodiscard]] explicit constexpr operator char16() const noexcept
            {
                return static_cast<char16>(intrinsic_value_);
            }

            /** Cast to char32. */
            [[nodiscard]] explicit constexpr operator char32() const noexcept
            {
                return static_cast<char32>(intrinsic_value_);
            }

            /** Cast to f32. */
            [[nodiscard]] explicit constexpr operator f32() const noexcept
            {
                return static_cast<f32>(intrinsic_value_);
            }

            /** Cast to f64. */
            [[nodiscard]] explicit constexpr operator f64() const noexcept
            {
                return static_cast<f64>(intrinsic_value_);
            }

            /** Cast to __int128. */
            [[nodiscard]] explicit constexpr operator __int128() const noexcept
            {
                return intrinsic_value_;
            }

            /** Cast to unsigned __int128. */
            [[nodiscard]] explicit constexpr operator unsigned __int128() const noexcept
            {
                return static_cast<unsigned __int128>(intrinsic_value_);
            }

            /** Checks whether other i128_intrinsics is equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ == other.intrinsic_value_;
            }

            /** Checks whether other i128_intrinsics is not equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ != other.intrinsic_value_;
            }

            /** Checks whether other i128_intrinsics is less than this */
            [[nodiscard]] constexpr bool operator<(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ < other.intrinsic_value_;
            }

            /** Checks whether other i128_intrinsics is less or equal than this */
            [[nodiscard]] constexpr bool operator<=(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ <= other.intrinsic_value_;
            }

            /** Checks whether other i128_intrinsics is greater than this */
            [[nodiscard]] constexpr bool operator>(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ > other.intrinsic_value_;
            }

            /** Checks whether other i128_intrinsics is greater or equal than this */
            [[nodiscard]] constexpr bool operator>=(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ >= other.intrinsic_value_;
            }

            /** Add this to other and return the result. */
            [[nodiscard]] constexpr i128_intrinsics operator+(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ + other.intrinsic_value_;
            }

            /** Add this to other and return this. */
            constexpr i128_intrinsics &operator+=(i128_intrinsics other) noexcept
            {
                *this = *this + other;
                return *this;
            }

            /** Subtract this to other and return the result. */
            [[nodiscard]] constexpr i128_intrinsics operator-(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ - other.intrinsic_value_;
            }

            /** Subtract this to other and return this. */
            constexpr i128_intrinsics &operator-=(i128_intrinsics other) noexcept
            {
                *this = *this - other;
                return *this;
            }

            /** Multiply this to other and return the result. */
            [[nodiscard]] constexpr i128_intrinsics operator*(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ * other.intrinsic_value_;
            }

            /** Multiply this to other and return this. */
            constexpr i128_intrinsics &operator*=(i128_intrinsics other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            /** Divide this to other and return the result. */
            [[nodiscard]] constexpr i128_intrinsics operator/(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ / other.intrinsic_value_;
            }

            /** Divide this to other and return this. */
            constexpr i128_intrinsics &operator/=(i128_intrinsics other) noexcept
            {
                *this = *this / other;
                return *this;
            }

            /** Retrieves the remainder of this divided by other and returns the result. */
            [[nodiscard]] constexpr i128_intrinsics operator%(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ % other.intrinsic_value_;
            }

            /** Retrieves the remainder of this divided by other, result in this and returns this. */
            constexpr i128_intrinsics &operator%=(i128_intrinsics other) noexcept
            {
                *this = *this % other;
                return *this;
            }

            /** Negate this. */
            [[nodiscard]] constexpr i128_intrinsics operator-() const noexcept
            {
                return -(intrinsic_value_);
            }

            /** Check if this is zero. */
            [[nodiscard]] constexpr bool operator!() const noexcept
            {
                return !(intrinsic_value_);
            }

            /** Invert this bits. */
            [[nodiscard]] constexpr i128_intrinsics operator~() const noexcept
            {
                return ~(intrinsic_value_);
            }

            /** Retrieves logical this OR other and return the result.*/
            [[nodiscard]] constexpr i128_intrinsics operator|(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ | other.intrinsic_value_;
            }

            /** Retrieves logical this AND other and return the result.*/
            [[nodiscard]] constexpr i128_intrinsics operator&(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ & other.intrinsic_value_;
            }

            /** Retrieves logical this XOR other and return the result.*/
            [[nodiscard]] constexpr i128_intrinsics operator^(i128_intrinsics other) const noexcept
            {
                return intrinsic_value_ ^ other.intrinsic_value_;
            }

            /** Logical OR */
            constexpr i128_intrinsics &operator|=(i128_intrinsics other) noexcept
            {
                intrinsic_value_ |= other.intrinsic_value_;
                return *this;
            }

            /** Logical AND */
            constexpr i128_intrinsics &operator&=(i128_intrinsics other) noexcept
            {
                intrinsic_value_ &= other.intrinsic_value_;
                return *this;
            }

            /** Logical XOR */
            constexpr i128_intrinsics &operator^=(i128_intrinsics other) noexcept
            {
                intrinsic_value_ ^= other.intrinsic_value_;
                return *this;
            }

            /** Post increment this. */
            constexpr i128_intrinsics operator++(i32) noexcept
            {
                i128_intrinsics tmp(*this);
                intrinsic_value_ += 1;
                return tmp;
            }

            /** Pre increment this. */
            constexpr i128_intrinsics &operator++() noexcept
            {
                intrinsic_value_ += 1;
                return *this;
            }

            /** Post decrement this. */
            constexpr i128_intrinsics operator--(i32) noexcept
            {
                i128_intrinsics tmp(*this);
                intrinsic_value_ -= 1;
                return tmp;
            }

            /** Pre decrement this. */
            constexpr i128_intrinsics &operator--() noexcept
            {
                intrinsic_value_ -= 1;
                return *this;
            }

            /** Left shift this and return the result */
            constexpr i128_intrinsics operator<<(i32 amount) const noexcept
            {
                return intrinsic_value_ << amount;
            }

            /** Right shift this and return the result */
            constexpr const i128_intrinsics operator>>(i32 amount) const noexcept
            {
                return intrinsic_value_ >> amount;
            }

            /** Left shift this and return the result */
            constexpr i128_intrinsics &operator<<=(i32 amount) noexcept
            {
                intrinsic_value_ <<= amount;
                return *this;
            }

            /** Right shift this and return the result */
            constexpr const i128_intrinsics &operator>>=(i32 amount) noexcept
            {
                intrinsic_value_ >>= amount;
                return *this;
            }

        private:
            __int128 intrinsic_value_;
        };

        using i128_impl = i128_intrinsics;

        struct alignas(16) u128_intrinsics
        {
            /** Default constructor. */
            constexpr u128_intrinsics() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_intrinsics(u64 high, u64 low) noexcept
                : intrinsic_value_ {static_cast<unsigned __int128>(high) << 64 | low}
            {
            }

            /** Construct a i128 from i32. */
            constexpr u128_intrinsics(i32 value) noexcept
                : intrinsic_value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from u32. */
            constexpr u128_intrinsics(u32 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr u128_intrinsics(i64 value) noexcept
                : intrinsic_value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from u64. */
            constexpr u128_intrinsics(u64 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr u128_intrinsics(__int128 value) noexcept
                : intrinsic_value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr u128_intrinsics(unsigned __int128 value) noexcept
                : intrinsic_value_ {value}
            {
            }

            /** Construct a i128 from i128. */
            explicit constexpr u128_intrinsics(i128_intrinsics value) noexcept
                : intrinsic_value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from f32. */
            constexpr u128_intrinsics(f32 value) noexcept
            {
                // Check value is not NaN or infinite
                HUD_CHECK(hud::math::is_finite(value));
                // Ensure we are positive
                HUD_CHECK(value > -1);
                intrinsic_value_ = static_cast<unsigned __int128>(value);
            }

            /** Construct a i128 from f64. */
            constexpr u128_intrinsics(f64 value) noexcept
            {
                // Check value is not NaN or infinite
                HUD_CHECK(hud::math::is_finite(value));
                // Ensure we are positive
                HUD_CHECK(value > -1);
                // Check value is lower than 2^128
                HUD_CHECK(value < hud::math::ldexp(static_cast<f64>(1), 128));
                intrinsic_value_ = static_cast<unsigned __int128>(value);
            }

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return static_cast<u64>(intrinsic_value_ & ~u64 {0});
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr u64 high() const noexcept
            {
                return static_cast<u64>(intrinsic_value_ >> 64);
            }

            /** Cast to bool. */
            [[nodiscard]] explicit constexpr operator bool() const noexcept
            {
                return intrinsic_value_ != 0u;
            }

            /** Cast to i8. */
            [[nodiscard]] explicit constexpr operator i8() const noexcept
            {
                return static_cast<i8>(intrinsic_value_);
            }

            /** Cast to u8. */
            [[nodiscard]] explicit constexpr operator u8() const noexcept
            {
                return static_cast<u8>(intrinsic_value_);
            }

            /** Cast to i16. */
            [[nodiscard]] explicit constexpr operator i16() const noexcept
            {
                return static_cast<i16>(intrinsic_value_);
            }

            /** Cast to u16. */
            [[nodiscard]] explicit constexpr operator u16() const noexcept
            {
                return static_cast<u16>(intrinsic_value_);
            }

            /** Cast to i32. */
            [[nodiscard]] explicit constexpr operator i32() const noexcept
            {
                return static_cast<i32>(intrinsic_value_);
            }

            /** Cast to u32. */
            [[nodiscard]] explicit constexpr operator u32() const noexcept
            {
                return static_cast<u32>(intrinsic_value_);
            }

            /** Cast to i64. */
            [[nodiscard]] explicit constexpr operator i64() const noexcept
            {
                return static_cast<i64>(intrinsic_value_);
            }

            /** Cast to u64. */
            [[nodiscard]] explicit constexpr operator u64() const noexcept
            {
                return intrinsic_value_;
            }

            /** Cast to ansichar. */
            [[nodiscard]] explicit constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(intrinsic_value_);
            }

            /** Cast to wchar. */
            [[nodiscard]] explicit constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(intrinsic_value_);
            }

            /** Cast to char16. */
            [[nodiscard]] explicit constexpr operator char16() const noexcept
            {
                return static_cast<char16>(intrinsic_value_);
            }

            /** Cast to char32. */
            [[nodiscard]] explicit constexpr operator char32() const noexcept
            {
                return static_cast<char32>(intrinsic_value_);
            }

            /** Cast to f32. */
            [[nodiscard]] explicit constexpr operator f32() const noexcept
            {
                return static_cast<f32>(intrinsic_value_);
            }

            /** Cast to f64. */
            [[nodiscard]] explicit constexpr operator f64() const noexcept
            {
                return static_cast<f64>(intrinsic_value_);
            }

            /** Cast to __int128. */
            [[nodiscard]] explicit constexpr operator __int128() const noexcept
            {
                return bit_cast_to_signed_int128(static_cast<__int128>(intrinsic_value_));
            }

            /** Cast to unsigned __int128. */
            [[nodiscard]] explicit constexpr operator unsigned __int128() const noexcept
            {
                return intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ == other.intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is not equal to this */
            [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ != other.intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is less than this */
            [[nodiscard]] constexpr bool operator<(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ < other.intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is less or equal than this */
            [[nodiscard]] constexpr bool operator<=(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ <= other.intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is greater than this */
            [[nodiscard]] constexpr bool operator>(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ > other.intrinsic_value_;
            }

            /** Checks whether other u128_intrinsics is greater or equal than this */
            [[nodiscard]] constexpr bool operator>=(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ >= other.intrinsic_value_;
            }

            /** Add this to other and return the result. */
            [[nodiscard]] constexpr u128_intrinsics operator+(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ + other.intrinsic_value_;
            }

            /** Add this to other and return this. */
            constexpr u128_intrinsics &operator+=(u128_intrinsics other) noexcept
            {
                *this = *this + other;
                return *this;
            }

            /** Subtract this to other and return the result. */
            [[nodiscard]] constexpr u128_intrinsics operator-(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ - other.intrinsic_value_;
            }

            /** Subtract this to other and return this. */
            constexpr u128_intrinsics &operator-=(u128_intrinsics other) noexcept
            {
                *this = *this - other;
                return *this;
            }

            /** Multiply this to other and return the result. */
            [[nodiscard]] constexpr u128_intrinsics operator*(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ * other.intrinsic_value_;
            }

            /** Multiply this to other and return this. */
            constexpr u128_intrinsics &operator*=(u128_intrinsics other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            /** Divide this to other and return the result. */
            [[nodiscard]] constexpr u128_intrinsics operator/(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ / other.intrinsic_value_;
            }

            /** Divide this to other and return this. */
            constexpr u128_intrinsics &operator/=(u128_intrinsics other) noexcept
            {
                *this = *this / other;
                return *this;
            }

            /** Retrieves the remainder of this divided by other and returns the result. */
            [[nodiscard]] constexpr u128_intrinsics operator%(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ % other.intrinsic_value_;
            }

            /** Retrieves the remainder of this divided by other, result in this and returns this. */
            constexpr u128_intrinsics &operator%=(u128_intrinsics other) noexcept
            {
                *this = *this % other;
                return *this;
            }

            /** Negate this. */
            [[nodiscard]] constexpr u128_intrinsics operator-() const noexcept
            {
                return -(intrinsic_value_);
            }

            /** Check if this is zero. */
            [[nodiscard]] constexpr bool operator!() const noexcept
            {
                return !(intrinsic_value_);
            }

            /** Invert this bits. */
            [[nodiscard]] constexpr u128_intrinsics operator~() const noexcept
            {
                return ~(intrinsic_value_);
            }

            /** Retrieves logical this OR other and return the result.*/
            [[nodiscard]] constexpr u128_intrinsics operator|(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ | other.intrinsic_value_;
            }

            /** Retrieves logical this AND other and return the result.*/
            [[nodiscard]] constexpr u128_intrinsics operator&(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ & other.intrinsic_value_;
            }

            /** Retrieves logical this XOR other and return the result.*/
            [[nodiscard]] constexpr u128_intrinsics operator^(u128_intrinsics other) const noexcept
            {
                return intrinsic_value_ ^ other.intrinsic_value_;
            }

            /** Logical OR */
            constexpr u128_intrinsics &operator|=(u128_intrinsics other) noexcept
            {
                intrinsic_value_ |= other.intrinsic_value_;
                return *this;
            }

            /** Logical AND */
            constexpr u128_intrinsics &operator&=(u128_intrinsics other) noexcept
            {
                intrinsic_value_ &= other.intrinsic_value_;
                return *this;
            }

            /** Logical XOR */
            constexpr u128_intrinsics &operator^=(u128_intrinsics other) noexcept
            {
                intrinsic_value_ ^= other.intrinsic_value_;
                return *this;
            }

            /** Post increment this. */
            constexpr u128_intrinsics operator++(i32) noexcept
            {
                u128_intrinsics tmp(*this);
                intrinsic_value_ += 1;
                return tmp;
            }

            /** Pre increment this. */
            constexpr u128_intrinsics &operator++() noexcept
            {
                intrinsic_value_ += 1;
                return *this;
            }

            /** Post decrement this. */
            constexpr u128_intrinsics operator--(i32) noexcept
            {
                u128_intrinsics tmp(*this);
                intrinsic_value_ -= 1;
                return tmp;
            }

            /** Pre decrement this. */
            constexpr u128_intrinsics &operator--() noexcept
            {
                intrinsic_value_ -= 1;
                return *this;
            }

            /** Left shift this and return the result */
            constexpr u128_intrinsics operator<<(i32 amount) const noexcept
            {
                return intrinsic_value_ << amount;
            }

            /** Right shift this and return the result */
            constexpr const u128_intrinsics operator>>(i32 amount) const noexcept
            {
                return intrinsic_value_ >> amount;
            }

            /** Left shift this and return the result */
            constexpr u128_intrinsics &operator<<=(i32 amount) noexcept
            {
                intrinsic_value_ <<= amount;
                return *this;
            }

            /** Right shift this and return the result */
            constexpr const u128_intrinsics &operator>>=(i32 amount) noexcept
            {
                intrinsic_value_ >>= amount;
                return *this;
            }

        private:
            unsigned __int128 intrinsic_value_;
        };

        using u128_impl = u128_intrinsics;

        /** Construct a i128 from i128. */
        constexpr i128_intrinsics::i128_intrinsics(u128_intrinsics value) noexcept
            : intrinsic_value_(static_cast<__int128>(value))
        {
        }

    } // namespace details::i128
} // namespace hud

#endif // HD_INC_CORE_I128_I128_INTRINSICS_H