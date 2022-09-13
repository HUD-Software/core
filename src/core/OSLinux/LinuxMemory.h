#pragma once
#ifndef HD_INC_OSLAYER_OS_LINUX_MEMORY_H
#define HD_INC_OSLAYER_OS_LINUX_MEMORY_H
#include "../OSCommon/CommonMemory.h"

#if !defined(HD_OS_LINUX)
#error This file must be included only when targetting Linux OS
#endif 

namespace hud::OS::Linux {

    struct Memory : public OS::Common::Memory {
        /**
        * Reverses the order of bytes in an integer.
        * Convert from little-endian to big-endian conversion and inverse.
        * @param value The integer
        * @param The integer the order of bytes reversed
        */
        static constexpr u32 reverse(const u32 value) noexcept {
           return __builtin_bswap32(value);
        }

        /**
        * Reverses the order of bytes in an integer.
        * Convert from little-endian to big-endian conversion and inverse.
        * @param value The integer
        * @param The integer the order of bytes reversed
        */
        static constexpr u64 reverse(const u64 value) noexcept {
            return __builtin_bswap64(value);
        }

        /**
        * Performs a rotation of a 32 bits value to the left.
        * @param value The 32 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u32 rotate_left(const u32 value, const u32 shift) noexcept {
            return __builtin_rotateleft32(value, shift);
        }

        /**
        * Performs a rotation of a 64 bits value to the left.
        * @param value The 64 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u64 rotate_left(const u64 value, const u32 shift) noexcept {
            return __builtin_rotateleft64(value, shift);
        }

        /**
        * Performs a rotation of a 32 bits value to the right.
        * @param value The 32 bits value to be rotated
        * @param shift The number of bits to shift
        * @return The rotated value
        */
        static constexpr u32 rotate_right(const u32 value, const u32 shift) noexcept {
            return __builtin_rotateright32(value, shift);
        }

        /**
        Performs a rotation of a 64 bits value to the right
        @param value The 64 bits value to be rotated
        @param shift The number of bits to shift
        @return The rotated value
        */
        static constexpr u64 rotate_right(const u64 value, const u32 shift) noexcept {
            return __builtin_rotateright64(value, shift);
        }
    };

} // hud::OS::Linux

#endif /* HD_INC_OSLAYER_OS_LINUX_MEMORY_H */