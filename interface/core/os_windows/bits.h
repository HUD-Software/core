#ifndef HD_INC_CORE_OS_WINDOWS_BITS_H
#define HD_INC_CORE_OS_WINDOWS_BITS_H
#include "../os_common/bits.h"

#if !defined(HD_OS_WINDOWS)
    #error This file must be included only when targetting Windows OS
#endif
namespace hud::os::windows
{
    struct bits : public os::common::bits
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
        [[nodiscard]] static constexpr u32 leading_zero(u32 value) noexcept
        {
            if (value == 0)
                return 32;

#if defined(HD_COMPILER_CLANG_CL)
            return __builtin_clz(value);
#elif defined(HD_COMPILER_MSVC)

            u32 result = 0;
            if (_BitScanReverse((unsigned long *)&result, value))
            {
                return 31 - result;
            }
            return 32;
#else
            return os::common::bits(value);
#endif
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zero(u64 value) noexcept
        {
            if (value == 0)
                return 64;

#if defined(HD_COMPILER_CLANG_CL)
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
            return os::common::bits(value);
#endif
        }
    };
} // namespace hud::os::windows

#endif // HD_INC_CORE_OS_WINDOWS_BITS_H