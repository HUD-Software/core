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

            __int128 intrinsic_value_;
        };

        /**
         * Checks whether right and left i128_intrinsics are equal.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if right and left i128_intrinsics are equal, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ == right.intrinsic_value_;
        }

        /**
         * Checks whether right and left i128_intrinsics are not equal.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if right and left i128_intrinsics are not equal, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ != right.intrinsic_value_;
        }

        /**
         * Checks whether left is less than right i128_intrinsics.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if left is less than right, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator<(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ < right.intrinsic_value_;
        }

        /**
         * Checks whether left is less or equal than right i128_intrinsics.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if left is less or equal than right, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator<=(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ <= right.intrinsic_value_;
        }

        /**
         * Checks whether left is greater than right i128_intrinsics.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if left is greater than right, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator>(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ > right.intrinsic_value_;
        }

        /**
         * Checks whether left is greater or equal than right i128_intrinsics.
         * @param left The left i128_intrinsics to compare
         * @param right The right i128_intrinsics to compare
         * @param true if left is greater or equal than right, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator>=(i128_intrinsics left, i128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ >= right.intrinsic_value_;
        }

        /** Add left to right and return the result. */
        [[nodiscard]] constexpr i128_intrinsics operator+(i128_intrinsics left, i128_intrinsics right)
        {
            return left.intrinsic_value_ + right.intrinsic_value_;
        }

        /** Subtract rght to left and return the result. */
        [[nodiscard]] constexpr i128_intrinsics operator-(i128_intrinsics left, i128_intrinsics right)
        {
            return left.intrinsic_value_ - right.intrinsic_value_;
        }

        /** Multiply left to right and return the result. */
        [[nodiscard]] constexpr i128_intrinsics operator*(i128_intrinsics left, i128_intrinsics right)
        {
            return left.intrinsic_value_ * right.intrinsic_value_;
        }

        /** Divide left to right and return the result. */
        [[nodiscard]] constexpr i128_intrinsics operator/(i128_intrinsics left, i128_intrinsics right)
        {
            return left.intrinsic_value_ / right.intrinsic_value_;
        }

        /** Retrieves the remainder of left divided by right and returns the result. */
        [[nodiscard]] constexpr i128_intrinsics operator%(i128_intrinsics left, i128_intrinsics right)
        {
            return left.intrinsic_value_ % right.intrinsic_value_;
        }

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
                hud::check(hud::math::is_finite(value));
                // Ensure we are positive
                hud::check(value > -1);
                intrinsic_value_ = static_cast<unsigned __int128>(value);
            }

            /** Construct a i128 from f64. */
            constexpr u128_intrinsics(f64 value) noexcept
            {
                // Check value is not NaN or infinite
                hud::check(hud::math::is_finite(value));
                // Ensure we are positive
                hud::check(value > -1);
                // Check value is lower than 2^128
                hud::check(value < hud::math::ldexp(static_cast<f64>(1), 128));
                intrinsic_value_ = static_cast<unsigned __int128>(value);
            }

            ///////////////////////////
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

            unsigned __int128 intrinsic_value_;
        };

        /**
         * Checks whether right and left u128_intrinsics are equal.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if right and left u128_intrinsics are equal, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(u128_intrinsics left, u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ == right.intrinsic_value_;
        }

        /**
         * Checks whether right and left u128_intrinsics are not equal.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if right and left u128_intrinsics are not equal, false otherwise
         */
        [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(i128_intrinsics left, u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ != right.intrinsic_value_;
        }

        /**
         * Checks whether left is less than right u128_intrinsics.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if left is less than right, false otherwise
         */
        [[nodiscard]] constexpr bool operator<(u128_intrinsics left, const u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ < right.intrinsic_value_;
        }

        /**
         * Checks whether left is less or equal than right u128_intrinsics.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if left is less or equal than right, false otherwise
         */
        [[nodiscard]] constexpr bool operator<=(u128_intrinsics left, u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ <= right.intrinsic_value_;
        }

        /**
         * Checks whether left is greater than right u128_intrinsics.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if left is greater than right, false otherwise
         */
        [[nodiscard]] constexpr bool operator>(u128_intrinsics left, u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ > right.intrinsic_value_;
        }

        /**
         * Checks whether left is greater or equal than right u128_intrinsics.
         * @param left The left u128_intrinsics to compare
         * @param right The right u128_intrinsics to compare
         * @param true if left is greater or equal than right, false otherwise
         */
        [[nodiscard]] constexpr bool operator>=(u128_intrinsics left, u128_intrinsics right) noexcept
        {
            return left.intrinsic_value_ >= right.intrinsic_value_;
        }

        /** Add left to right and return the result. */
        [[nodiscard]] constexpr u128_intrinsics operator+(u128_intrinsics left, u128_intrinsics right)
        {
            return left.intrinsic_value_ + right.intrinsic_value_;
        }

        /** Subtract rght to left and return the result. */
        [[nodiscard]] constexpr u128_intrinsics operator-(u128_intrinsics left, u128_intrinsics right)
        {
            return left.intrinsic_value_ - right.intrinsic_value_;
        }

        /** Multiply left to right and return the result. */
        [[nodiscard]] constexpr u128_intrinsics operator*(u128_intrinsics left, u128_intrinsics right)
        {
            return left.intrinsic_value_ * right.intrinsic_value_;
        }

        /** Divide left to right and return the result. */
        [[nodiscard]] constexpr u128_intrinsics operator/(u128_intrinsics left, u128_intrinsics right)
        {
            return left.intrinsic_value_ / right.intrinsic_value_;
        }

        /** Retrieves the remainder of left divided by right and returns the result. */
        [[nodiscard]] constexpr u128_intrinsics operator%(u128_intrinsics left, u128_intrinsics right)
        {
            return left.intrinsic_value_ % right.intrinsic_value_;
        }

        using u128_impl = u128_intrinsics;

        constexpr i128_intrinsics::i128_intrinsics(u128_intrinsics value) noexcept
            : intrinsic_value_(static_cast<__int128>(value))
        {
        }

    } // namespace details::i128
} // namespace hud

#endif // HD_INC_CORE_I128_I128_INTRINSICS_H