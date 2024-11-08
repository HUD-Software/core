#ifndef HD_INC_CORE_HASH_H
#define HD_INC_CORE_HASH_H
#include "traits/is_enum.h"
#include "traits/is_pointer.h"
#include "traits/underlying_type.h"
#include "traits/is_integral.h"
#include "hash/city_hash.h"
#include "templates/bit_cast.h"
#include <core/cstring.h>

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
    [[nodiscard]] static constexpr u32 hash_32(const ansichar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(value, length);
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static constexpr u32 hash_32(const ansichar *const value) noexcept
    {
        return hash_32(value, hud::cstring::length(value));
    }

    /** Retrieves the 32 bits hash of a wchar null-terminated string. */
    [[nodiscard]] static inline u32 hash_32(const wchar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_32(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static inline u32 hash_32(const wchar *const value) noexcept
    {
        return hash_32(value, hud::cstring::length(value));
    }

    /** Retrieves the 32 bits hash of an enumeration. */
    template<typename type_t>
    requires(is_enum_v<type_t>)
    [[nodiscard]] static constexpr u32 hash_32(const type_t value) noexcept
    {
        return hash_32(static_cast<underlying_type_t<type_t>>(value));
    }

    /** Retrieves the 32 bits hash of a pointer of a type type_t. */
    [[nodiscard]] static inline u32 hash_32(const void *const pointer) noexcept
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
    [[nodiscard]] static constexpr u32 combine_32(u32 a, u32 b) noexcept
    {
        return hud::hash_algorithm::city_hash::combine_32(a, b);
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
    [[nodiscard]] static constexpr u64 hash_64(const ansichar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_64(value, length);
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static constexpr u64 hash_64(const ansichar *const value) noexcept
    {
        return hash_64(value, hud::cstring::length(value));
    }

    /** Retrieves the 64 bits hash of a wchar null-terminated string. */
    [[nodiscard]] static inline u64 hash_64(const wchar *value, usize length) noexcept
    {
        return hud::hash_algorithm::city_hash::hash_64(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
    }

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    [[nodiscard]] static inline u64 hash_64(const wchar *const value) noexcept
    {
        return hash_64(value, hud::cstring::length(value));
    }

    /** Retrieves the 64 bits hash of an enumeration. */
    template<typename type_t>
    requires(is_enum_v<type_t>)
    [[nodiscard]] static constexpr u64 hash_64(const type_t value) noexcept
    {
        return hash_64(static_cast<underlying_type_t<type_t>>(value));
    }

    /** Retrieves the 64 bits hash of a pointer of a type type_t. */
    [[nodiscard]] static inline u64 hash_64(const void *const pointer) noexcept
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
    [[nodiscard]] static constexpr u64 combine_64(u64 a, u64 b) noexcept
    {
        return hud::hash_algorithm::city_hash::combine_64(a, b);
    }

    /**
     * A 32 bit hasher class used for hashing an arbitrary stream of bytes
     * Instances of `hasher_32` usually represent state that is changed while hashing data.
     * `hasher_32` provides a fairly basic interface for retrieving the generated hash.
     * `hasher_32` used all functions in `hud` namespace like `hud::hash_32(const u32 value)`, `hud::hash_32(const ansichar *const value)` or `hud::combine_32(u64 a, u64 b)`.
     *  If you want to hash a user defined type, add your `hud::hash_my_type(my_type& t)` function and just call `hasher_32::operator()`
     */
    class hasher_32
    {
    public:
        /** Hash the value and combine the value with the current hasher value. */
        template<typename... type_t>
        [[nodiscard]] constexpr hasher_32 &operator()(type_t &&...values) noexcept
        {
            state_ = hud::combine_32(state_, hud::hash_32(hud::forward<type_t>(values)...));
            return *this;
        }

        /** Retrieves the value of the `hasher_32`. */
        [[nodiscard]] operator u32() const noexcept
        {
            return state_;
        }

        /** Hash the value and combine the value with the current hasher value. */
        template<typename... type_t>
        [[nodiscard]] constexpr hasher_32 &hash(type_t &&...values) noexcept
        {
            return (*this)(hud::forward<type_t>(values)...);
        }

    private:
        u32 state_ {0}; // Default is 0, but can be a seed
    };

    /**
     * A 64 bit hasher class used for hashing an arbitrary stream of bytes
     * Instances of `hasher_64` usually represent state that is changed while hashing data.
     * `hasher_64` provides a fairly basic interface for retrieving the generated hash.
     * `hasher_64` used all functions in `hud` namespace like `hud::hash_64(const u32 value)`, `hud::hash_64(const ansichar *const value)` or `hud::combine_64(u64 a, u64 b)`.
     *  If you want to hash a user defined type, add your `hud::hash_my_type(my_type& t)` function and just call `hasher_64::operator()`
     */
    struct hasher_64
    {
    public:
        /** Hash the value and combine the value with the current hasher value. */
        template<typename... type_t>
        [[nodiscard]] constexpr hasher_64 &operator()(type_t &&...values) noexcept
        {
            state_ = hud::combine_64(state_, hud::hash_64(hud::forward<type_t>(values)...));
            return *this;
        }

        /** Retrieves the value of the `hasher_64`. */
        [[nodiscard]] operator u64() const noexcept
        {
            return state_;
        }

        /** Hash the value and combine the value with the current hasher value. */
        template<typename... type_t>
        [[nodiscard]] constexpr hasher_64 &hash(type_t &&...values) noexcept
        {
            return (*this)(hud::forward<type_t>(values)...);
        }

    private:
        u64 state_ {0}; // Default is 0, but can be a seed
    };

} // namespace hud

#endif // HD_INC_CORE_HASH_H
