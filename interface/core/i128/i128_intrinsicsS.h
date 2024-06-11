#ifndef HD_INC_CORE_I128_I128_INTRINSICS_H
#define HD_INC_CORE_I128_I128_INTRINSICS_H

#if !defined(HD_INTRINSIC_INT128_SUPPORTED)
    #error i128_intrinsics.h is included but HD_INTRINSIC_INT128_SUPPORTED is not set
#endif

namespace hud
{
    class i128;
    class u128;

    namespace details::i128
    {
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
        constexpr __int128 bit_cast_to_signed_int128(unsigned __int128 value)
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
        constexpr i64 bit_cast_to_signed_i64(u64 v)
        {
            return v & (u64 {1} << 63) ? ~static_cast<i64>(~v) : static_cast<i64>(v);
        }

        struct alignas(16) i128_intrinsics
        {
            /** Default constructor. */
            constexpr i128_intrinsics() = default;

            /** Construct a i128 from low and high part. */
            constexpr i128_intrinsics(i64 high, u64 low) noexcept
                : value_ {bit_cast_to_signed_int128(static_cast<unsigned __int128>(high) << 64 | low)}
            {
            }

            /** Construct a i128 from i32. */
            constexpr i128_intrinsics(i32 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from u32. */
            constexpr i128_intrinsics(u32 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr i128_intrinsics(i64 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from u64. */
            constexpr i128_intrinsics(u64 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr i128_intrinsics(__int128 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr i128_intrinsics(unsigned __int128 value) noexcept
                : value_ {static_cast<__int128>(value)}
            {
            }

            /** Construct a i128 from i128. */
            explicit constexpr i128_intrinsics(hud::u128 value) noexcept;

            /** Construct a i128 from f32. */
            i128_intrinsics(f32 value) noexcept
                : value_(value)
            {
            }

            /** Construct a i128 from f64. */
            i128_intrinsics(f64 value) noexcept
                : value_(value)
            {
            }

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return static_cast<u64>(value_ & ~u64 {0});
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr i64 high() const noexcept
            {
                return bit_cast_to_signed_i64(static_cast<u64>(static_cast<unsigned __int128>(value_) >> 64));
            }

            /** Cast to bool. */
            [[nodiscard]] constexpr operator bool() const noexcept
            {
                return value_ != 0u;
            }

            /** Cast to i8. */
            [[nodiscard]] constexpr operator i8() const noexcept
            {
                return static_cast<i8>(value_);
            }

            /** Cast to u8. */
            [[nodiscard]] constexpr operator u8() const noexcept
            {
                return static_cast<u8>(value_);
            }

            /** Cast to i16. */
            [[nodiscard]] constexpr operator i16() const noexcept
            {
                return static_cast<i16>(value_);
            }

            /** Cast to u16. */
            [[nodiscard]] constexpr operator u16() const noexcept
            {
                return static_cast<u16>(value_);
            }

            /** Cast to i32. */
            [[nodiscard]] constexpr operator i32() const noexcept
            {
                return static_cast<i32>(value_);
            }

            /** Cast to u32. */
            [[nodiscard]] constexpr operator u32() const noexcept
            {
                return static_cast<u32>(value_);
            }

            /** Cast to i64. */
            [[nodiscard]] constexpr operator i64() const noexcept
            {
                return static_cast<i64>(value_);
            }

            /** Cast to u64. */
            [[nodiscard]] constexpr operator u64() const noexcept
            {
                return value_;
            }

            /** Cast to ansichar. */
            [[nodiscard]] constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(value_);
            }

            /** Cast to wchar. */
            [[nodiscard]] constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(value_);
            }

            /** Cast to char16. */
            [[nodiscard]] constexpr operator char16() const noexcept
            {
                return static_cast<char16>(value_);
            }

            /** Cast to char32. */
            [[nodiscard]] constexpr operator char32() const noexcept
            {
                return static_cast<char32>(value_);
            }

            /** Cast to f32. */
            [[nodiscard]] constexpr operator f32() const noexcept
            {
                return static_cast<f32>(value_);
            }

            /** Cast to f64. */
            [[nodiscard]] constexpr operator f64() const noexcept
            {
                return static_cast<f64>(value_);
            }

            /** Cast to __int128. */
            [[nodiscard]] constexpr operator __int128() const noexcept
            {
                return value_;
            }

            /** Cast to unsigned __int128. */
            [[nodiscard]] constexpr operator unsigned __int128() const noexcept
            {
                return static_cast<unsigned __int128>(value_);
            }

            __int128 value_;
        };

        using i128_impl = i128_intrinsics;

        struct alignas(16) u128_intrinsics
        {
            /** Default constructor. */
            constexpr u128_intrinsics() = default;

            /** Construct a u128 from low and high part. */
            constexpr u128_intrinsics(u64 high, u64 low) noexcept
                : value_ {static_cast<unsigned __int128>(high) << 64 | low}
            {
            }

            /** Construct a i128 from i32. */
            constexpr u128_intrinsics(i32 value) noexcept
                : value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from u32. */
            constexpr u128_intrinsics(u32 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from i64. */
            constexpr u128_intrinsics(i64 value) noexcept
                : value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from u64. */
            constexpr u128_intrinsics(u64 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr u128_intrinsics(__int128 value) noexcept
                : value_ {static_cast<unsigned __int128>(value)}
            {
            }

            /** Construct a i128 from __int128. */
            constexpr u128_intrinsics(unsigned __int128 value) noexcept
                : value_ {value}
            {
            }

            /** Construct a i128 from i128. */
            explicit constexpr u128_intrinsics(hud::i128 value) noexcept;

            /** Construct a i128 from f32. */
            constexpr u128_intrinsics(f32 value) noexcept
                : value_(value)
            {
            }

            /** Construct a i128 from f64. */
            constexpr u128_intrinsics(f64 value) noexcept
                : value_(value)
            {
            }

            /** Retrieves the low part of the u64. */
            [[nodiscard]] constexpr u64 low() const noexcept
            {
                return static_cast<u64>(value_ & ~u64 {0});
            }

            /** Retrieves the high part of the i64. */
            [[nodiscard]] constexpr u64 high() const noexcept
            {
                return static_cast<u64>(value_ >> 64);
            }

            /** Cast to bool. */
            [[nodiscard]] constexpr operator bool() const noexcept
            {
                return value_ != 0u;
            }

            /** Cast to i8. */
            [[nodiscard]] constexpr operator i8() const noexcept
            {
                return static_cast<i8>(value_);
            }

            /** Cast to u8. */
            [[nodiscard]] constexpr operator u8() const noexcept
            {
                return static_cast<u8>(value_);
            }

            /** Cast to i16. */
            [[nodiscard]] constexpr operator i16() const noexcept
            {
                return static_cast<i16>(value_);
            }

            /** Cast to u16. */
            [[nodiscard]] constexpr operator u16() const noexcept
            {
                return static_cast<u16>(value_);
            }

            /** Cast to i32. */
            [[nodiscard]] constexpr operator i32() const noexcept
            {
                return static_cast<i32>(value_);
            }

            /** Cast to u32. */
            [[nodiscard]] constexpr operator u32() const noexcept
            {
                return static_cast<u32>(value_);
            }

            /** Cast to i64. */
            [[nodiscard]] constexpr operator i64() const noexcept
            {
                return static_cast<i64>(value_);
            }

            /** Cast to u64. */
            [[nodiscard]] constexpr operator u64() const noexcept
            {
                return value_;
            }

            /** Cast to ansichar. */
            [[nodiscard]] constexpr operator ansichar() const noexcept
            {
                return static_cast<ansichar>(value_);
            }

            /** Cast to wchar. */
            [[nodiscard]] constexpr operator wchar() const noexcept
            {
                return static_cast<wchar>(value_);
            }

            /** Cast to char16. */
            [[nodiscard]] constexpr operator char16() const noexcept
            {
                return static_cast<char16>(value_);
            }

            /** Cast to char32. */
            [[nodiscard]] constexpr operator char32() const noexcept
            {
                return static_cast<char32>(value_);
            }

            /** Cast to f32. */
            [[nodiscard]] constexpr operator f32() const noexcept
            {
                return static_cast<f32>(value_);
            }

            /** Cast to f64. */
            [[nodiscard]] constexpr operator f64() const noexcept
            {
                return static_cast<f64>(value_);
            }

            /** Cast to __int128. */
            [[nodiscard]] constexpr operator __int128() const noexcept
            {
                return bit_cast_to_signed_int128(static_cast<__int128>(value_));
            }

            /** Cast to unsigned __int128. */
            [[nodiscard]] constexpr operator unsigned __int128() const noexcept
            {
                return value_;
            }

            unsigned __int128 value_;
        };

        using u128_impl = u128_intrinsics;
    } // namespace details::i128
} // namespace hud

#endif // HD_INC_CORE_I128_I128_INTRINSICS_H