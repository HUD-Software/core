#ifndef HD_INC_CORE_BITS_BITS_WINDOWS_H
#define HD_INC_CORE_BITS_BITS_WINDOWS_H
#include "bits_common.h"

#if !defined(HD_HOST_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif
namespace hud::windows
{
    struct bits : public hud::common::bits
    {
        /**
         * Reverses the order of bytes in an integer.
         * Convert from little-endian to big-endian conversion and inverse.
         * @param value The integer
         * @param The integer the order of bytes reversed
         */
        static constexpr u32 reverse_bytes(const u32 value) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                u32 tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
                return (tmp << 16) | (tmp >> 16);
            }
            else
            {
                return _byteswap_ulong(value);
            }
        }

        /**
         * Reverses the order of bytes in an integer.
         * Convert from little-endian to big-endian conversion and inverse.
         * @param value The integer
         * @param The integer the order of bytes reversed
         */
        static constexpr u64 reverse_bytes(const u64 value) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                u64 tmp = value;
                tmp = ((tmp & 0x00000000FFFFFFFFull) << 32) | ((tmp & 0xFFFFFFFF00000000ull) >> 32);
                tmp = ((tmp & 0x0000FFFF0000FFFFull) << 16) | ((tmp & 0xFFFF0000FFFF0000ull) >> 16);
                tmp = ((tmp & 0x00FF00FF00FF00FFull) << 8) | ((tmp & 0xFF00FF00FF00FF00ull) >> 8);
                return tmp;
            }
            else
            {
                return _byteswap_uint64(value);
            }
        }

        /**
         * Performs a rotation of a 32 bits value to the left.
         * @param value The 32 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u32 rotate_left(const u32 value, const u32 shift) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                if (shift == 0)
                {
                    return value;
                }
                return (value << shift) | (value >> (32 - shift));
            }
            else
            {
                return _rotl(value, static_cast<i32>(shift));
            }
        }

        /**
         * Performs a rotation of a 64 bits value to the left.
         * @param value The 64 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u64 rotate_left(const u64 value, const u32 shift) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                if (shift == 0)
                {
                    return value;
                }
                return (value << shift) | (value >> (64 - shift));
            }
            else
            {
                return _rotl64(value, static_cast<i32>(shift));
            }
        }

        /**
         * Performs a rotation of a 32 bits value to the right.
         * @param value The 32 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u32 rotate_right(const u32 value, const u32 shift) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                if (shift == 0)
                {
                    return value;
                }
                return (value >> shift) | (value << (32 - shift));
            }
            else
            {
                return _rotr(value, static_cast<i32>(shift));
            }
        }

        /**
        Performs a rotation of a 64 bits value to the right
        @param value The 64 bits value to be rotated
        @param shift The number of bits to shift
        @return The rotated value
        */
        static constexpr u64 rotate_right(const u64 value, const u32 shift) noexcept
        {
            if (hud::is_constant_evaluated())
            {
                if (shift == 0)
                {
                    return value;
                }
                return (value >> shift) | (value << (64 - shift));
            }
            else
            {
                return _rotr64(value, static_cast<i32>(shift));
            }
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zeros(u8 value) noexcept
        {
            return leading_zeros(u32 {value}) - 24;
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zeros(u16 value) noexcept
        {
#if HD_HAS_BUILTIN_CLZS
            return value == 0 ? 16 : __builtin_clzs(value);
#else
            return leading_zeros(u32 {value}) - 16;
#endif
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zeros(u32 value) noexcept
        {
#if HD_HAS_BUILTIN_CLZ
            return value == 0 ? 32 : __builtin_clz(value);
#else
            if (value == 0)
                return 32;
            if (hud::is_constant_evaluated())
                return hud::common::bits::leading_zeros(value);
            u32 result = 0;
            if (_BitScanReverse((unsigned long *)&result, value))
            {
                return 31 - result;
            }
            return 32;
#endif
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zeros(u64 value) noexcept
        {
#if HD_HAS_BUILTIN_CLZLL
            return value == 0 ? 64 : __builtin_clzll(value);
#else
            if (value == 0)
                return 64;
            if (hud::is_constant_evaluated())
                return hud::common::bits::leading_zeros(value);
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
#endif
        }

        [[nodiscard]] static constexpr u32 trailing_zeros(u8 value) noexcept
        {
            return value == 0 ? 8 : trailing_zeros(u32 {value});
        }

        [[nodiscard]] static constexpr u32 trailing_zeros(u16 value) noexcept
        {
            return value == 0 ? 16 : trailing_zeros(u32 {value});
        }

        [[nodiscard]] static constexpr u32 trailing_zeros(u32 value) noexcept
        {
#if HD_HAS_BUILTIN_CTZ
            return value == 0 ? 32 : __builtin_ctz(value);
#else
            if (value == 0)
                return 32;
            if (hud::is_constant_evaluated())
                return hud::common::bits::trailing_zeros(value);
            u32 result = 0;
            _BitScanForward((unsigned long *)&result, value);
            return result;
#endif
        }

        [[nodiscard]] static constexpr u64 trailing_zeros(u64 value) noexcept
        {
#if HD_HAS_BUILTIN_CTZLL
            return value == 0 ? 64 : __builtin_ctzll(value);
#else
            if (value == 0)
                return 64;
            if (hud::is_constant_evaluated())
                return hud::common::bits::trailing_zeros(value);
    #if defined(HD_TARGET_X64)
            u64 result = 0;
            _BitScanForward64((unsigned long *)&result, value);
            return result;
    #else
            u32 result = 0;
            if (static_cast<u32>(value) == 0)
            {
                _BitScanForward((unsigned long *)&result, static_cast<u32>(value >> 32));
                return result + 32;
            }
            _BitScanForward((unsigned long *)&result, static_cast<u32>(value));
            return result;
    #endif
#endif
        }
    };
} // namespace hud::windows

#endif // HD_INC_CORE_BITS_BITS_WINDOWS_H