#ifndef HD_INC_CORE_HASH_H
#define HD_INC_CORE_HASH_H
#include "traits/is_enum.h"
#include "traits/is_pointer.h"
#include "traits/underlying_type.h"
#include "traits/is_integral.h"
#include "hash/city_hash.h"
#include "templates/bit_cast.h"

namespace hud
{

    template<typename type_t>
    static inline u32 hash_32(const type_t) noexcept;

    /** Retrieves the 32 bits hash of a i8 value. */
    [[nodiscard]] static constexpr u32 hash_32(const i8 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u8 value. */
    [[nodiscard]] static constexpr u32 hash_32(const u8 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i16 value. */
    [[nodiscard]] static constexpr u32 hash_32(const i16 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u16 value. */
    [[nodiscard]] static constexpr u32 hash_32(const u16 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i32 value. */
    [[nodiscard]] static constexpr u32 hash_32(const i32 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u32 value. */
    [[nodiscard]] static constexpr u32 hash_32(const u32 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i64 value. */
    [[nodiscard]] static constexpr u32 hash_32(const i64 value) noexcept
    {
        // We multiply high part with prime number 23
        // (The product of a prime with any other number has the best chance of being unique
        // due to the fact that a prime is used to compose it
        return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
    }

    /** Retrieves the 32 bits hash of a u64 value. */
    [[nodiscard]] static constexpr u32 hash_32(const u64 value) noexcept
    {
        // We multiply high part with prime number 23
        // (The product of a prime with any other number has the best chance of being unique
        // due to the fact that a prime is used to compose it
        return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
    }

    /** Retrieves the 32 bits hash of a f32 value. */
    [[nodiscard]] static constexpr u32 hash_32(const f32 value) noexcept
    {
        return bit_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a f64 value. */
    [[nodiscard]] static constexpr u32 hash_32(const f64 value) noexcept
    {
        return hash_32(bit_cast<u64>(value));
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static inline u32 hash_32(const ansichar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(value, length);
    }

    /** Retrieves the 32 bits hash of a wchar null-terminated string. */
    [[nodiscard]] static inline u32 hash_32(const wchar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
    }

    /** Retrieves the 32 bits hash of an enumeration. */
    template<typename type_t>
    requires(is_enum_v<type_t>)
    [[nodiscard]] static constexpr u32 hash_32(const type_t value) noexcept
    {
        return hash_32(static_cast<underlying_type_t<type_t>>(value));
    }

    /** Retrieves the 32 bits hash of a pointer of a type type_t. */
    template<typename type_t>
    [[nodiscard]] static u32 hash_32(type_t *const pointer) noexcept
    {
        const uptr ptr = reinterpret_cast<uptr>(pointer);
        if constexpr (sizeof(uptr) == 4)
        {
            return hash_32(static_cast<u32>(ptr));
        }
        else if constexpr (sizeof(uptr) == 8)
        {
            return hash_32(static_cast<u32>(ptr >> 4)); // 4 lowest bits in 64 bits are likely zero, ignore them
        }
    }

    /** Combine two 32 bits value. */
    static u64 combine_32(u64 a, u64 b)
    {
        return hud::hash_algorithm::city_hash::combine(a, b);
    }

    template<typename type_t>
    static inline u64 hash_64(const type_t) noexcept;

    /** Retrieves the 64 bits hash of a i8 value. */
    [[nodiscard]] static constexpr u64 hash_64(const i8 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a u8 value. */
    [[nodiscard]] static constexpr u64 hash_64(const u8 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a i16 value. */
    [[nodiscard]] static constexpr u64 hash_64(const i16 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a u16 value. */
    [[nodiscard]] static constexpr u64 hash_64(const u16 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a i32 value. */
    [[nodiscard]] static constexpr u64 hash_64(const i32 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a u32 value. */
    [[nodiscard]] static constexpr u64 hash_64(const u32 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a i64 value. */
    [[nodiscard]] static constexpr u64 hash_64(const i64 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a u64 value. */
    [[nodiscard]] static constexpr u64 hash_64(const u64 value) noexcept
    {
        return static_cast<u64>(value);
    }

    /** Retrieves the 64 bits hash of a f32 value. */
    [[nodiscard]] static constexpr u64 hash_64(const f32 value) noexcept
    {
        return static_cast<u64>(bit_cast<u32>(value));
    }

    /** Retrieves the 64 bits hash of a f64 value. */
    [[nodiscard]] static constexpr u64 hash_64(const f64 value) noexcept
    {
        return hash_64(bit_cast<u64>(value));
    }

    /** Retrieves the 64 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static inline u64 hash_64(const ansichar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_64(value, length);
    }

    /** Retrieves the 64 bits hash of a wchar null-terminated string. */
    [[nodiscard]] static inline u64 hash_64(const wchar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_64(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
    }

    /** Retrieves the 64 bits hash of an enumeration. */
    template<typename type_t>
    requires(is_enum_v<type_t>)
    [[nodiscard]] static constexpr u64 hash_64(const type_t value) noexcept
    {
        return hash_64(static_cast<underlying_type_t<type_t>>(value));
    }

    /** Retrieves the 64 bits hash of a pointer of a type type_t. */
    template<typename type_t>
    [[nodiscard]] static u64 hash_64(type_t *const pointer) noexcept
    {
        const uptr ptr = reinterpret_cast<uptr>(pointer);
        if constexpr (sizeof(uptr) == 4)
        {
            return hash_64(static_cast<u64>(ptr));
        }
        else if constexpr (sizeof(uptr) == 8)
        {
            return hash_64(static_cast<u64>(ptr >> 4)); // 4 lowest bits in 64 bits are likely zero, ignore them
        }
    }

    /** Combine two 64 bits value. */
    static u64 combine_64(u64 a, u64 b)
    {
        // From abseil 64 bit hash mix
        /** Unsigned 128 bits value */
        // TODO: Replace with hud::u128
        struct u128
        {
            u64 low;
            u64 high;
        };

        static constexpr uint64_t kMul = sizeof(size_t) == 4 ? uint64_t {0xcc9e2d51} : uint64_t {0x9ddfea08eb382d69};
        using MulResultType = hud::conditional_t<sizeof(uptr) == 4, u64, u128>;
        // We do the addition in 64-bit space to make sure the 128-bit
        // multiplication is fast. If we were to do it as MultType the compiler has
        // to assume that the high word is non-zero and needs to perform 2
        // multiplications instead of one.
        // MulResultType m = a + b;
        // m *= kMul;
        // return static_cast<uint64_t>(m ^ (m >> (sizeof(m) * 8 / 2)));
    }
} // namespace hud

#endif // HD_INC_CORE_HASH_H
