#pragma once
#ifndef HD_INC_OSLAYER_OS_WINDOWS_MEMORY_H
#define HD_INC_OSLAYER_OS_WINDOWS_MEMORY_H
#include "../OSCommon/CommonMemory.h"

#if !defined(HD_OS_WINDOWS)
#error This file must be included only when targetting Windows OS
#endif 

namespace hud::OS::Windows{

    struct Memory : public OS::Common::Memory {

        /**
        * Reverses the order of bytes in an integer.
        * Convert from little-endian to big-endian conversion and inverse.
        * @param value The integer
        * @param The integer the order of bytes reversed
        */
        static constexpr u32 reverse(const u32 value) noexcept {
            if (is_constant_evaluated()) {
                u32 tmp = ((value << 8) & 0xFF00FF00) | ((value >> 8) & 0xFF00FF);
                return (tmp << 16) | (tmp >> 16);
            }
            else {
                return _byteswap_ulong(value);
            }
        }

        /**
        * Reverses the order of bytes in an integer.
        * Convert from little-endian to big-endian conversion and inverse.
        * @param value The integer
        * @param The integer the order of bytes reversed
        */
        static constexpr u64 reverse(const u64 value) noexcept {
            if (is_constant_evaluated()) {
                u64 tmp = value;
                tmp = ((tmp & 0x00000000FFFFFFFFull) << 32) | ((tmp & 0xFFFFFFFF00000000ull) >> 32);
                tmp = ((tmp & 0x0000FFFF0000FFFFull) << 16) | ((tmp & 0xFFFF0000FFFF0000ull) >> 16);
                tmp = ((tmp & 0x00FF00FF00FF00FFull) << 8) | ((tmp & 0xFF00FF00FF00FF00ull) >> 8);
                return tmp;
            }
            else {
                return _byteswap_uint64(value);
            }
        }

        /**
        * Performs a rotation of a 32 bits value to the left.
        * @param value The 32 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u32 rotate_left(const u32 value, const u32 shift) noexcept {
            if (is_constant_evaluated()) {
                if (shift == 0) {
                    return value;
                }
                return (value << shift) | (value >> (32 - shift));
            }
            else {
                return _rotl(value, static_cast<i32>(shift));
            }
        }

        /**
        * Performs a rotation of a 64 bits value to the left.
        * @param value The 64 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u64 rotate_left(const u64 value, const u32 shift) noexcept {
            if (is_constant_evaluated()) {
                if (shift == 0) {
                    return value;
                }
                return (value << shift) | (value >> (64 - shift));
            }
            else {
                return _rotl64(value, static_cast<i32>(shift));
            }
        }

        /**
        * Performs a rotation of a 32 bits value to the right.
        * @param value The 32 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u32 rotate_right(const u32 value, const u32 shift) noexcept {
            if (is_constant_evaluated()) {
                if (shift == 0) {
                    return value;
                }
                return (value >> shift) | (value << (32 - shift));
            }
            else {
                return _rotr(value, static_cast<i32>(shift));
            }
        }

        /**
        Performs a rotation of a 64 bits value to the right
        @param value The 64 bits value to be rotated
        @param shift The number of bits to shift
        @return The rotated value
        */
        static constexpr u64 rotate_right(const u64 value, const u32 shift) noexcept {
            if (is_constant_evaluated()) {
                if (shift == 0) {
                    return value;
                }
                return (value >> shift) | (value << (64 - shift));
            }
            else {
                return _rotr64(value, static_cast<i32>(shift));
            }
        }
    };

} // namespace hud::OS::Windows

#endif // HD_INC_OSLAYER_OS_WINDOWS_MEMORY_H

