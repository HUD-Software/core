// Copyright (c) 2011 Google, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// CityHash, by Geoff Pike and Jyrki Alakuijala
//
// http://code.google.com/p/cityhash/
//
// This file provides a few functions for hashing strings.  All of them are
// high-quality functions in the sense that they pass standard tests such
// as Austin Appleby's SMHasher.  They are also fast.
//
// For 64-bit x86 code, on short strings, we don't know of anything faster than
// CityHash64 that is of comparable quality.  We believe our nearest competitor
// is Murmur3.  For 64-bit x86 code, CityHash64 is an excellent choice for hash
// tables and most other hashing (excluding cryptography).
//
// For 64-bit x86 code, on long strings, the picture is more complicated.
// On many recent Intel CPUs, such as Nehalem, Westmere, Sandy Bridge, etc.,
// CityHashCrc128 appears to be faster than all competitors of comparable
// quality.  CityHash128 is also good but not quite as fast.  We believe our
// nearest competitor is Bob Jenkins' Spooky.  We don't have great data for
// other 64-bit CPUs, but for long strings we know that Spooky is slightly
// faster than CityHash on some relatively recent AMD x86-64 CPUs, for example.
// Note that CityHashCrc128 is declared in citycrc.h.
//
// For 32-bit x86 code, we don't know of anything faster than CityHash32 that
// is of comparable quality.  We believe our nearest competitor is Murmur3A.
// (On 64-bit CPUs, it is typically faster to use the other CityHash variants.)
//
// Functions in the CityHash family are not suitable for cryptography.
//
// Please see CityHash's README file for more details on our performance
// measurements and so on.
//
// WARNING: This code has been only lightly tested on big-endian platforms!
// It is known to work well on little-endian platforms that have a small penalty
// for unaligned reads, such as current Intel and AMD moderate-to-high-end CPUs.
// It should work on all 32-bit and 64-bit platforms that allow unaligned reads;
// bug reports are welcome.
//
// By the way, for some hash functions, given strings a and b, the hash
// of a+b is easily derived from the hashes of a and b.  This property
// doesn't hold for any hash functions in this file.

#ifndef HD_INC_CORE_HASH_ALGORITHM_CITY_HASH_H
#define HD_INC_CORE_HASH_ALGORITHM_CITY_HASH_H
#include "../bits.h"
#include "../memory.h"
#include "../templates/swap.h"
#include "../templates/bit_cast.h"

namespace hud::hash_algorithm
{

    namespace details
    {
        /** Prime number used by CityHash */
        static constexpr u64 K0 = 0xc3a5c85c97cb3127ULL;
        static constexpr u64 K1 = 0xb492b66fbe98f273ULL;
        static constexpr u64 K2 = 0x9ae16a3b2f90404fULL;

        /** Magic Murmur3 values */
        static constexpr u32 C1 = 0xcc9e2d51;
        static constexpr u32 C2 = 0x1b873593;

        /**
         * Performs CityHash64 shift and mix
         * @param value The value
         * @param The shift and mixed value
         */
        [[nodiscard]] static constexpr u64 shift_mix(u64 value) noexcept
        {
            return value ^ (value >> 47);
        }

        /**
         * Performs a 32 bits murmur3 avalanche mix hash
         * @param key The key to mix
         * @return The resulting value
         */
        [[nodiscard]] static constexpr u32 avalanche_mixer(u32 key) noexcept
        {
            key ^= key >> 16;
            key *= 0x85ebca6b;
            key ^= key >> 13;
            key *= 0xc2b2ae35;
            key ^= key >> 16;
            return key;
        }

        /**
         * Combine two 32 bits value with murmur3
         * @param a The first value to combine
         * @param b The second value to combine
         * @return The combined value
         */
        [[nodiscard]] constexpr u32 combine_32(u32 a, u32 b) noexcept
        {
            a *= C1;
            a = hud::bits::rotate_left(a, 15);
            a *= C2;
            b ^= a;
            b = hud::bits::rotate_left(b, 13);
            return b * 5 + 0xe6546b64;
        }

        /**
         * Performs a load of 32 bits into an aligned memory from a unaligned memory
         * @param buffer The possibly unaligned memory
         * @return The 32 bits aligned memory
         */
        [[nodiscard]] static constexpr u32 fetch_32(const ansichar *buffer) noexcept
        {
            if constexpr (compilation::is_endianness(endianness::big))
            {
                return hud::bits::reverse_bytes(hud::memory::unaligned_load32(buffer)); // LCOV_EXCL_LINE
            }
            else
            {
                return hud::memory::unaligned_load32(buffer);
            }
        }

        /**
         * Performs a load of 32 bits into an aligned memory from a unaligned memory
         * @param buffer The possibly unaligned memory
         * @return The 32 bits aligned memory
         */
        [[nodiscard]] static constexpr u64 fetch_64(const ansichar *buffer) noexcept
        {
            if constexpr (compilation::is_endianness(endianness::big))
            {
                return hud::bits::reverse_bytes(hud::memory::unaligned_load64(buffer)); // LCOV_EXCL_LINE
            }
            else
            {
                return hud::memory::unaligned_load64(buffer);
            }
        }

        /**
         * Retrives the 32 bits hash value for a key with length between 0 and 4 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u32 hash_32_len_0_to_4(const ansichar *key, usize length) noexcept
        {
            u32 b = 0;
            u32 c = 9;
            for (usize i = 0; i < length; i++)
            {
                u8 v = static_cast<u8>(key[i]);
                b = b * C1 + v;
                c ^= b;
            }
            return avalanche_mixer(
                combine_32(b, combine_32(static_cast<u32>(length), c))
            );
        }

        /**
         * Retrives the 32 bits hash value for a key with length between 5 and 12 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u32 hash_32_len_5_to_12(const ansichar *key, usize length) noexcept
        {
            u32 a = static_cast<u32>(length), b = static_cast<u32>(length) * 5, c = 9, d = b;
            a += fetch_32(key);
            b += fetch_32(key + length - 4);
            c += fetch_32(key + ((length >> 1) & 4));
            return avalanche_mixer(
                combine_32(c, combine_32(b, combine_32(a, d)))
            );
        }

        /**
         * Retrives the 32 bits hash value for a key with length between 13 and 24 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u32 hash_32_len_13_to_24(const ansichar *key, usize length) noexcept
        {
            u32 a = fetch_32(key - 4 + (length >> 1));
            u32 b = fetch_32(key + 4);
            u32 c = fetch_32(key + length - 8);
            u32 d = fetch_32(key + (length >> 1));
            u32 e = fetch_32(key);
            u32 f = fetch_32(key + length - 4);
            u32 h = static_cast<u32>(length);

            return avalanche_mixer(
                combine_32(f, combine_32(e, combine_32(d, combine_32(c, combine_32(b, combine_32(a, h))))))
            );
        }

        /**
         * Retrives the 64 bits hash value of 2 values with a multiplier
         * @param value1 The first value
         * @param value2 The second value
         * @param multiplier The multiplier
         * @return Value1 combined with Value2
         */
        [[nodiscard]] static constexpr u64 hash_64_len_16(u64 value1, u64 value2, u64 multiplier) noexcept
        {
            // Murmur-inspired hashing.
            u64 a = (value1 ^ value2) * multiplier;
            a ^= (a >> 47);
            u64 b = (value2 ^ a) * multiplier;
            b ^= (b >> 47);
            b *= multiplier;
            return b;
        }

        /**
         * Retrives the 64 bits hash value for a key with length between 0 and 16 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u64 hash_64_len_0_to_16(const ansichar *key, usize length) noexcept
        {
            if (length >= 8)
            {
                u64 mul = K2 + length * 2;
                u64 a = fetch_64(key) + K2;
                u64 b = fetch_64(key + length - 8);
                u64 c = hud::bits::rotate_right(b, 37) * mul + a;
                u64 d = (hud::bits::rotate_right(a, 25) + b) * mul;
                return hash_64_len_16(c, d, mul);
            }
            if (length >= 4)
            {
                u64 mul = K2 + length * 2;
                u64 a = fetch_32(key);
                return hash_64_len_16(length + (a << 3), fetch_32(key + length - 4), mul);
            }
            if (length > 0)
            {
                u8 a = static_cast<u8>(key[0]);
                u8 b = static_cast<u8>(key[length >> 1]);
                u8 c = static_cast<u8>(key[length - 1]);
                u32 y = static_cast<u32>(a) + (static_cast<u32>(b) << 8);
                u32 z = static_cast<u32>(length) + (static_cast<u32>(c) << 2);
                return shift_mix(y * K2 ^ z * K0) * K2;
            }
            return K2;
        }

        /**
         * Retrives the 64 bits hash value for a key with length between 17 and 32 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u64 hash_64_len_17_to_32(const ansichar *key, usize length) noexcept
        {
            u64 mul = K2 + length * 2;
            u64 a = fetch_64(key) * K1;
            u64 b = fetch_64(key + 8);
            u64 c = fetch_64(key + length - 8) * mul;
            u64 d = fetch_64(key + length - 16) * K2;
            return hash_64_len_16(hud::bits::rotate_right(a + b, 43) + hud::bits::rotate_right(c, 30) + d, a + hud::bits::rotate_right(b + K2, 18) + c, mul);
        }

        /**
         * Retrives the 64 bits hash value for a key with length between 33 and 64 bytes included
         * @param key The key to hash
         * @param length The length of the key
         * @return The hash of the key
         */
        [[nodiscard]] static constexpr u64 hash_64_len_33_to_64(const ansichar *key, usize len) noexcept
        {
            u64 mul = K2 + len * 2;
            u64 a = fetch_64(key) * K2;
            u64 b = fetch_64(key + 8);
            u64 c = fetch_64(key + len - 24);
            u64 d = fetch_64(key + len - 32);
            u64 e = fetch_64(key + 16) * K2;
            u64 f = fetch_64(key + 24) * 9;
            u64 g = fetch_64(key + len - 8);
            u64 h = fetch_64(key + len - 16) * mul;
            u64 u = hud::bits::rotate_right(a + g, 43) + (hud::bits::rotate_right(b, 30) + c) * 9;
            u64 v = ((a + g) ^ d) + f + 1;
            u64 w = hud::bits::reverse_bytes((u + v) * mul) + h;
            u64 x = hud::bits::rotate_right(e + f, 42) + c;
            u64 y = (hud::bits::reverse_bytes((v + w) * mul) + g) * mul;
            u64 z = e + f + c;
            a = hud::bits::reverse_bytes((x + z) * mul + y) + b;
            b = shift_mix((z + a) * mul + d + h) * mul;
            return b + x;
        }

        /**
         * Retrives the 64 bits hash value form a 128 bits hash
         * @param hash The 128bits hash
         * @return The 64 bits hash
         */
        [[nodiscard]] static constexpr u64 hash_128_to_64(const u128 &hash) noexcept
        {
            // Murmur-inspired hashing.
            const u64 kMul = 0x9ddfea08eb382d69ULL;
            u64 a = (hash.low() ^ hash.high()) * kMul;
            a ^= (a >> 47);
            u64 b = (hash.high() ^ a) * kMul;
            b ^= (b >> 47);
            b *= kMul;
            return b;
        }

        /**
         * Performs hash of 128 bits
         * @param low The 64 bits low value
         * @param high The 64 bits high value
         * @return The 64 bits hash
         */
        [[nodiscard]] static constexpr u64 hash_64_len_16(u64 low, u64 high) noexcept
        {
            return hash_128_to_64(u128 {high, low});
        }

        /**
         * Performs a 128 bits hash for 48 bytes. Quick and dirty.
         * Callers do best to use "random-looking" values for a and b.
         * @param w 8 bytes to hash
         * @param x 8 bytes to hash
         * @param y 8 bytes to hash
         * @param z 8 bytes to hash
         * @param a 8 bytes to hash
         * @param b 8 bytes to hash
         * @return 128 bits hash of 48 bytes
         */
        [[nodiscard]] static constexpr u128 weak_hash_len_32_with_seeds(u64 w, u64 x, u64 y, u64 z, u64 a, u64 b) noexcept
        {
            a += w;
            b = hud::bits::rotate_right(b + a + z, 21);
            u64 c = a;
            a += x;
            a += y;
            b += hud::bits::rotate_right(a, 44);
            return u128 {b + c, a + z};
        }

        /**
         * Performs a 128 bits hash for 48 bytes. Quick and dirty.
         * Return a 16-byte hash for key[0] ... key[31], a, and b.
         * @param key 32 bytes
         * @param a 8 bytes
         * @param b 8 bytes
         * @return 128 bits hash of 48 bytes
         */
        [[nodiscard]] static constexpr u128 weak_hash_len_32_with_seeds(const ansichar *key, u64 a, u64 b) noexcept
        {
            return weak_hash_len_32_with_seeds(fetch_64(key), fetch_64(key + 8), fetch_64(key + 16), fetch_64(key + 24), a, b);
        }

        /**
         * Combine two 64 bits value
         * @param a The first value to combine
         * @param b The second value to combine
         * @return The combined value
         */
        [[nodiscard]] static constexpr u64 combine_64(u64 a, u64 b) noexcept
        {
            return hash_128_to_64(u128 {a, b});
        }

    } // namespace details

    struct city_hash
    {

        /**
         * Combine two 32 bits value with murmur3
         * @param a The first value to combine
         * @param b The second value to combine
         * @return The 32 bits combined value
         */
        [[nodiscard]] static constexpr u32 combine_32(u32 a, u32 b) noexcept
        {
            return details::combine_32(a, b);
        }

        /**
         * Performs a 32 bytes hash of a buffer using cityhash algorithm
         * @param buffer The buffer to hash
         * @param length Length of the key in bytes
         * @return The 32 bits hash of the key
         */
        [[nodiscard]] static constexpr u32 hash_32(const ansichar *buffer, usize length) noexcept
        {
            if (length <= 24)
            {
                return length <= 12 ? (length <= 4 ? details::hash_32_len_0_to_4(buffer, length) : details::hash_32_len_5_to_12(buffer, length)) : details::hash_32_len_13_to_24(buffer, length);
            }

            // length > 24
            u32 h = static_cast<u32>(length), g = details::C1 * static_cast<u32>(length), f = g;
            u32 a0 = hud::bits::rotate_right(details::fetch_32(buffer + length - 4) * details::C1, 17) * details::C2;
            u32 a1 = hud::bits::rotate_right(details::fetch_32(buffer + length - 8) * details::C1, 17) * details::C2;
            u32 a2 = hud::bits::rotate_right(details::fetch_32(buffer + length - 16) * details::C1, 17) * details::C2;
            u32 a3 = hud::bits::rotate_right(details::fetch_32(buffer + length - 12) * details::C1, 17) * details::C2;
            u32 a4 = hud::bits::rotate_right(details::fetch_32(buffer + length - 20) * details::C1, 17) * details::C2;
            h ^= a0;
            h = hud::bits::rotate_right(h, 19);
            h = h * 5 + 0xe6546b64;
            h ^= a2;
            h = hud::bits::rotate_right(h, 19);
            h = h * 5 + 0xe6546b64;
            g ^= a1;
            g = hud::bits::rotate_right(g, 19);
            g = g * 5 + 0xe6546b64;
            g ^= a3;
            g = hud::bits::rotate_right(g, 19);
            g = g * 5 + 0xe6546b64;
            f += a4;
            f = hud::bits::rotate_right(f, 19);
            f = f * 5 + 0xe6546b64;
            usize iters = (length - 1) / 20;
            do
            {
                u32 a0_ = hud::bits::rotate_right(details::fetch_32(buffer) * details::C1, 17) * details::C2;
                u32 a1_ = details::fetch_32(buffer + 4);
                u32 a2_ = hud::bits::rotate_right(details::fetch_32(buffer + 8) * details::C1, 17) * details::C2;
                u32 a3_ = hud::bits::rotate_right(details::fetch_32(buffer + 12) * details::C1, 17) * details::C2;
                u32 a4_ = details::fetch_32(buffer + 16);
                h ^= a0_;
                h = hud::bits::rotate_right(h, 18);
                h = h * 5 + 0xe6546b64;
                f += a1_;
                f = hud::bits::rotate_right(f, 19);
                f = f * details::C1;
                g += a2_;
                g = hud::bits::rotate_right(g, 18);
                g = g * 5 + 0xe6546b64;
                h ^= a3_ + a1_;
                h = hud::bits::rotate_right(h, 19);
                h = h * 5 + 0xe6546b64;
                g ^= a4_;
                g = hud::bits::reverse_bytes(g) * 5;
                h += a4_ * 5;
                h = hud::bits::reverse_bytes(h);
                f += a0_;
                swap(f, h);
                swap(f, g);
                buffer += 20;
            } while (--iters != 0);
            g = hud::bits::rotate_right(g, 11) * details::C1;
            g = hud::bits::rotate_right(g, 17) * details::C1;
            f = hud::bits::rotate_right(f, 11) * details::C1;
            f = hud::bits::rotate_right(f, 17) * details::C1;
            h = hud::bits::rotate_right(h + g, 19);
            h = h * 5 + 0xe6546b64;
            h = hud::bits::rotate_right(h, 17) * details::C1;
            h = hud::bits::rotate_right(h + f, 19);
            h = h * 5 + 0xe6546b64;
            h = hud::bits::rotate_right(h, 17) * details::C1;
            return h;
        }

        /**
         * Performs a 64 bytes hash of a buffer using cityhash algorithm
         * @param buffer The buffer to hash
         * @param length Length of the key in bytes
         * @return The 64 bits hash of the buffer
         */
        [[nodiscard]] static constexpr u64 hash_64(const ansichar *buffer, usize length) noexcept
        {
            if (length <= 32)
            {
                if (length <= 16)
                {
                    return details::hash_64_len_0_to_16(buffer, length);
                }
                else
                {
                    return details::hash_64_len_17_to_32(buffer, length);
                }
            }
            else if (length <= 64)
            {
                return details::hash_64_len_33_to_64(buffer, length);
            }

            // For strings over 64 bytes we hash the end first, and then as we
            // loop we keep 56 bytes of state: v, w, x, y, and z.
            u64 x = details::fetch_64(buffer + length - 40);
            u64 y = details::fetch_64(buffer + length - 16) + details::fetch_64(buffer + length - 56);
            u64 z = details::hash_64_len_16(details::fetch_64(buffer + length - 48) + length, details::fetch_64(buffer + length - 24));
            u128 v = details::weak_hash_len_32_with_seeds(buffer + length - 64, length, z);
            u128 w = details::weak_hash_len_32_with_seeds(buffer + length - 32, y + details::K1, x);
            x = x * details::K1 + details::fetch_64(buffer);

            // Decrease len to the nearest multiple of 64, and operate on 64-byte chunks.
            length = (length - 1) & ~static_cast<usize>(63);
            do
            {
                x = hud::bits::rotate_right(x + y + v.low() + details::fetch_64(buffer + 8), 37) * details::K1;
                y = hud::bits::rotate_right(y + v.high() + details::fetch_64(buffer + 48), 42) * details::K1;
                x ^= w.high();
                y += v.low() + details::fetch_64(buffer + 40);
                z = hud::bits::rotate_right(z + w.low(), 33) * details::K1;
                v = details::weak_hash_len_32_with_seeds(buffer, v.high() * details::K1, x + w.low());
                w = details::weak_hash_len_32_with_seeds(buffer + 32, z + w.high(), y + details::fetch_64(buffer + 16));
                swap(z, x);
                buffer += 64;
                length -= 64;
            } while (length != 0);
            return details::hash_64_len_16(details::hash_64_len_16(v.low(), w.low()) + details::shift_mix(y) * details::K1 + z, details::hash_64_len_16(v.high(), w.high()) + x);
        }

        /**
         * Combine two 64 bits value
         * @param a The first value to combine
         * @param b The second value to combine
         * @return The 64 bits combined value
         */
        [[nodiscard]] static constexpr u64 combine_64(u64 a, u64 b) noexcept
        {
            return details::combine_64(a, b);
        }
    };

} // namespace hud::hash_algorithm

#endif // HD_INC_CORE_HASH_ALGORITHM_CITY_HASH_H