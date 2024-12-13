#ifndef HD_INC_CORE_BITS_BITS_COMMON_H
#define HD_INC_CORE_BITS_BITS_COMMON_H

namespace hud::common
{
    struct bits
    {
        /** Reverse bits of a 32 bits integral type. */
        [[nodiscard]] static constexpr u32 reverse_bits(u32 integral) noexcept
        {
            integral = (integral << 16) | (integral >> 16);
            integral = ((integral & 0x00ff00ff) << 8) | ((integral & 0xff00ff00) >> 8);
            integral = ((integral & 0x0f0f0f0f) << 4) | ((integral & 0xf0f0f0f0) >> 4);
            integral = ((integral & 0x33333333) << 2) | ((integral & 0xcccccccc) >> 2);
            integral = ((integral & 0x55555555) << 1) | ((integral & 0xaaaaaaaa) >> 1);
            return integral;
        }

        /** Determines if a 64-bit word contains any zero byte using a SIMD-like technique
         * Example:
         *    value = 0x1213140015001617
         *    result = 0x0000008000800000
         *    This indicates that a zero byte (0x00) is present in the word at byte index 2 and 4
         */
        // [[nodiscard]] static constexpr u64 has_zero_byte(u64 value) noexcept
        // {
        //     // From http://graphics.stanford.edu/~seander/bithacks.html#ZeroInWord
        //     // value= 0x1213140015001617
        //     // value - 0x0101010101010101 = 0x111212FF13FF1516
        //     // ~value                    = 0xEDECEBFFEAFFE9E8
        //     // Final result after AND      = 0x0000008000800000
        //     return ((value)-0x0101010101010101UL) & ~(value) & 0x8080808080808080UL;
        // }

        /**
         * Determines if a 64-bit word contains any byte equal to a given value
         * using a SIMD-like technique
         *
         * Example:
         *    value = 0x1213140015001217
         *    byte_value = 0x12
         *    result = 0x8000000000008000
         *    This indicates that `byte_value` is present in the word at byte index 1 and 7
         */
        // [[nodiscard]] static constexpr u64 has_value_byte(u64 value, u8 byte_value) noexcept
        // {
        //     // From http://graphics.stanford.edu/~seander/bithacks.html#ValueInWord
        //     // value = 0x1213140015001217
        //     // byte_value = 0x12
        //     // 0x0101010101010101ULL * byte_value = 0x1212121212121212
        //     // value ^ (0x0101010101010101ULL * byte_value) = 0x0001061207120005
        //     // Final result after has_zero_byte = 0x8000000000008000
        //     return has_zero_byte(value ^ (0x0101010101010101ULL * byte_value));
        // }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zero(u8 value) noexcept
        {
            return leading_zero(u16 {value}) - 8;
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zero(u16 value) noexcept
        {
            return leading_zero(u32 {value}) - 16;
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zero(u32 value) noexcept
        {
            return value == 0 ? 32 : 31 - hud::math::floor_log2(u32(value));
        }

        /** Returns the number of consecutive 0 bits in the value. */
        [[nodiscard]] static constexpr u32 leading_zero(u64 value) noexcept
        {
            return value == 0 ? 64 : 63 - hud::math::floor_log2(value);
        }

        [[nodiscard]] static constexpr u32 trailing_zero(u8 value) noexcept
        {
            u8 c = 8; // c will be the number of zero bits on the right
            value &= -i8(value);
            if (value)
                c--;
            if (value & 0x0Fu)
                c -= 4;
            if (value & 0x33u)
                c -= 2;
            if (value & 055u)
                c -= 1;
            return c;
        }

        [[nodiscard]] static constexpr u32 trailing_zero(u16 value) noexcept
        {
            u16 c = 16; // c will be the number of zero bits on the right
            value &= -i16(value);
            if (value)
                c--;
            if (value & 0x00FFu)
                c -= 8;
            if (value & 0x0F0Fu)
                c -= 4;
            if (value & 0x3333u)
                c -= 2;
            if (value & 0x5555u)
                c -= 1;
            return c;
        }

        [[nodiscard]] static constexpr u32 trailing_zero(u32 value) noexcept
        {
            u32 c = 32; // c will be the number of zero bits on the right
            value &= -i32(value);
            if (value)
                c--;
            if (value & 0x0000FFFFu)
                c -= 16;
            if (value & 0x00FF00FFu)
                c -= 8;
            if (value & 0x0F0F0F0Fu)
                c -= 4;
            if (value & 0x33333333u)
                c -= 2;
            if (value & 0x55555555u)
                c -= 1;
            return c;
        }

        [[nodiscard]] static constexpr u64 trailing_zero(u64 value) noexcept
        {
            u64 c = 64; // c will be the number of zero bits on the right
            value &= -i64(value);
            if (value)
                c--;
            if (value & 0x00000000FFFFFFFFu)
                c -= 32;
            if (value & 0x0000FFFF0000FFFFu)
                c -= 16;
            if (value & 0x00FF00FF00FF00FFu)
                c -= 8;
            if (value & 0x0F0F0F0F0F0F0F0Fu)
                c -= 4;
            if (value & 0x3333333333333333u)
                c -= 2;
            if (value & 0x5555555555555555u)
                c -= 1;
            return c;
        }

        /**Check if the given is a valid power of two mask like 0x00FFFFFF. */
        [[nodiscard]] static constexpr bool is_valid_power_of_two_mask(u64 value) noexcept
        {
            return ((value + 1) & value) == 0;
        }
    };

} // namespace hud::common

#endif // HD_INC_CORE_BITS_BITS_COMMON_H