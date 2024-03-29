#ifndef HD_INC_CORE_OS_LINUX_MEMORY_H
#define HD_INC_CORE_OS_LINUX_MEMORY_H
#include "../os_common/memory.h"

#if !defined(HD_OS_LINUX)
    #error This file must be included only when targetting Linux OS
#endif

namespace hud::os::linux
{

    struct memory : public os::common::memory
    {
        /**
         * Reverses the order of bytes in an integer.
         * Convert from little-endian to big-endian conversion and inverse.
         * @param value The integer
         * @param The integer the order of bytes reversed
         */
        static constexpr u32 reverse(const u32 value) noexcept
        {
#if HD_HAS_BUILTIN_BUILTIN_BSWAP32
            return __builtin_bswap32(value);
#else
    #error hud::os::linux::memory::reverse(u32) not defined
#endif
        }

        /**
         * Reverses the order of bytes in an integer.
         * Convert from little-endian to big-endian conversion and inverse.
         * @param value The integer
         * @param The integer the order of bytes reversed
         */
        static constexpr u64 reverse(const u64 value) noexcept
        {
#if HD_HAS_BUILTIN_BULITIN_BSWAP64
            return __builtin_bswap64(value);
#else
    #error hud::os::linux::memory::reverse(u64) not defined
#endif
        }

        /**
         * Performs a rotation of a 32 bits value to the left.
         * @param value The 32 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u32 rotate_left(const u32 value, const u32 shift) noexcept
        {
#if HD_HAS_BUILTIN_BULITIN_ROTATELEFT32
            return __builtin_rotateleft32(value, shift);
#else
            const u32 __r = shift % 32;
            return (value << __r) | (value >> ((32 - __r) % 32));
#endif
        }

        /**
         * Performs a rotation of a 64 bits value to the left.
         * @param value The 64 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u64 rotate_left(const u64 value, const u32 shift) noexcept
        {
#if HD_HAS_BUILTIN_BULITIN_ROTATELEFT64
            return __builtin_rotateleft64(value, shift);
#else
            const u64 __r = shift % 64;
            return (value << __r) | (value >> ((64 - __r) % 64));
#endif
        }

        /**
         * Performs a rotation of a 32 bits value to the right.
         * @param value The 32 bits value to be rotated
         * @param shift The number of bits to shift
         * @return The rotated value
         */
        static constexpr u32 rotate_right(const u32 value, const u32 shift) noexcept
        {
#if HD_HAS_BUILTIN_BULITIN_ROTATERIGHT32
            return __builtin_rotateright32(value, shift);
#else
            const u32 __r = shift % 32;
            return (value >> __r) | (value << ((32 - __r) % 32));
#endif
        }

        /**
        Performs a rotation of a 64 bits value to the right
        @param value The 64 bits value to be rotated
        @param shift The number of bits to shift
        @return The rotated value
        */
        static constexpr u64 rotate_right(const u64 value, const u32 shift) noexcept
        {
#if HD_HAS_BUILTIN_BULITIN_ROTATERIGHT64
            return __builtin_rotateright64(value, shift);
#else
            const u64 __r = shift % 64;
            return (value >> __r) | (value << ((64 - __r) % 64));
#endif
        }
    };

} // namespace hud::os::linux

#endif /* HD_INC_CORE_OS_LINUX_MEMORY_H */