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
    static inline u32 hash(const type_t) noexcept;

    /** Retrieves the 32 bits hash of a i8 value. */
    [[nodiscard]] static constexpr u32 hash(const i8 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u8 value. */
    [[nodiscard]] static constexpr u32 hash(const u8 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i16 value. */
    [[nodiscard]] static constexpr u32 hash(const i16 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u16 value. */
    [[nodiscard]] static constexpr u32 hash(const u16 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i32 value. */
    [[nodiscard]] static constexpr u32 hash(const i32 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a u32 value. */
    [[nodiscard]] static constexpr u32 hash(const u32 value) noexcept
    {
        return static_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a i64 value. */
    [[nodiscard]] static constexpr u32 hash(const i64 value) noexcept
    {
        // We multiply high part with prime number 23
        // (The product of a prime with any other number has the best chance of being unique
        // due to the fact that a prime is used to compose it
        return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
    }

    /** Retrieves the 32 bits hash of a u64 value. */
    [[nodiscard]] static constexpr u32 hash(const u64 value) noexcept
    {
        // We multiply high part with prime number 23
        // (The product of a prime with any other number has the best chance of being unique
        // due to the fact that a prime is used to compose it
        return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
    }

    /** Retrieves the 32 bits hash of a f32 value. */
    [[nodiscard]] static constexpr u32 hash(const f32 value) noexcept
    {
        return bit_cast<u32>(value);
    }

    /** Retrieves the 32 bits hash of a f64 value. */
    [[nodiscard]] static constexpr u32 hash(const f64 value) noexcept
    {
        return hash(bit_cast<u64>(value));
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static inline u32 hash(const ansichar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(value, length);
    }

    /** Retrieves the 32 bits hash of a wchar null-terminated string. */
    [[nodiscard]] static inline u32 hash(const wchar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
    }

    /** Retrieves the 32 bits hash of an enumeration. */
    template<typename type_t>
    requires(is_enum_v<type_t>)
    [[nodiscard]] static constexpr u32 hash(const type_t value) noexcept
    {
        return hash(static_cast<underlying_type_t<type_t>>(value));
    }

    /** Retrieves the 32 bits hash of a pointer of a type type_t. */
    template<typename type_t>
    [[nodiscard]] static u32 hash(type_t *const pointer) noexcept
    {
        const uptr ptr = reinterpret_cast<uptr>(pointer);
        if constexpr (sizeof(uptr) == 4)
        {
            return static_cast<u32>(ptr);
        }
        else if constexpr (sizeof(uptr) == 8)
        {
            return static_cast<u32>(ptr >> 4); // 4 lowest bits in 64 bits are likely zero, ignore them
        }
    }

} // namespace hud

#endif // HD_INC_CORE_HASH_H
