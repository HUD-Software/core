#ifndef HD_INC_CORE_HASH_H
#define HD_INC_CORE_HASH_H
#include "traits/is_enum.h"
#include "traits/is_pointer.h"
#include "traits/underlying_type.h"
#include "traits/is_integral.h"
#include "hash/city_hash.h"
#include "templates/bit_cast.h"
#include "cstring.h"
#include "traits/decay.h"
#include "templates/declval.h"

namespace hud
{
    template<typename... type_t>
    struct hash_32;

    /** Retrieves the 32 bits hash of a i8 value. */
    template<>
    struct hash_32<i8>
    {
        [[nodiscard]] constexpr u32 operator()(const i8 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a u8 value. */
    template<>
    struct hash_32<u8>
    {
        [[nodiscard]] constexpr u32 operator()(const u8 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a i16 value. */
    template<>
    struct hash_32<i16>
    {
        [[nodiscard]] constexpr u32 operator()(const i16 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a u16 value. */
    template<>
    struct hash_32<u16>
    {
        [[nodiscard]] constexpr u32 operator()(const u16 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a i32 value. */
    template<>
    struct hash_32<i32>
    {
        [[nodiscard]] constexpr u32 operator()(const i32 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a u32 value. */
    template<>
    struct hash_32<u32>
    {
        [[nodiscard]] constexpr u32 operator()(const u32 value) const
        {
            return static_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a i64 value. */
    template<>
    struct hash_32<i64>
    {
        [[nodiscard]] constexpr u32 operator()(const i64 value) const
        {
            // We multiply high part with prime number 23
            // (The product of a prime with any other number has the best chance of being unique
            // due to the fact that a prime is used to compose it
            return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
        }
    };

    /** Retrieves the 32 bits hash of a u64 value. */
    template<>
    struct hash_32<u64>
    {
        [[nodiscard]] constexpr u32 operator()(const u64 value) const
        {
            // We multiply high part with prime number 23
            // (The product of a prime with any other number has the best chance of being unique
            // due to the fact that a prime is used to compose it
            return static_cast<u32>(value) + (static_cast<u32>(value >> 32) * 23);
        }
    };

    /** Retrieves the 32 bits hash of a f32 value. */
    template<>
    struct hash_32<f32>
    {
        [[nodiscard]] constexpr u32 operator()(const f32 value) const
        {
            return hud::bit_cast<u32>(value);
        }
    };

    /** Retrieves the 32 bits hash of a f64 value. */
    template<>
    struct hash_32<f64>
    {
        [[nodiscard]] constexpr u32 operator()(const f64 value) const
        {
            return hash_32<u64> {}(hud::bit_cast<u64>(value));
        }
    };

    /** Retrieves the 32 bits hash of a ansichar null-terminated string. */
    template<>
    struct hash_32<const ansichar *, usize>
    {
        [[nodiscard]] constexpr u32 operator()(const ansichar *value, usize length) const
        {
            return hud::hash_algorithm::city_hash::hash_32(value, length);
        }
    };

    template<>
    struct hash_32<const ansichar *>
    {
        [[nodiscard]] constexpr u32 operator()(const ansichar *value, usize length) const
        {
            return hud::hash_32<const ansichar *, usize> {}(value, length);
        }
    };

    /** Retrieves the 32 bits hash of a wchar null-terminated string. */
    template<>
    struct hash_32<const wchar *, usize>
    {
        [[nodiscard]] inline u32 operator()(const wchar *value, usize length) const
        {
            return hud::hash_algorithm::city_hash::hash_32(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
        }
    };

    template<>
    struct hash_32<const wchar *>
    {
        [[nodiscard]] inline u32 operator()(const wchar *value, usize length) const
        {
            return hud::hash_32<const wchar *, usize> {}(value, length);
        }
    };

    /** Retrieves the 32 bits hash of a pointer of a type type_t. */
    template<>
    struct hash_32<const void *>
    {
        [[nodiscard]] inline u32 operator()(const void *const pointer) const
        {
            const uptr ptr = reinterpret_cast<uptr>(pointer);
            if constexpr (sizeof(uptr) == 4)
            {
                return hash_32<u32> {}(static_cast<u32>(ptr));
            }
            else if constexpr (sizeof(uptr) == 8)
            {
                return hash_32<u64> {}(static_cast<u32>(ptr >> 4)); // 4 lowest bits in 64 bits are likely zero, ignore them
            }
        }
    };

    /** Combine two 32 bits value. */
    [[nodiscard]] static constexpr u32 combine_32(u32 a, u32 b) noexcept
    {
        return hud::hash_algorithm::city_hash::combine_32(a, b);
    }

    template<typename... type_t>
    struct hash_64;

    /** Retrieves the 64 bits hash of a i8 value. */
    template<>
    struct hash_64<i8>
    {
        [[nodiscard]] constexpr u64 operator()(const i8 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a u8 value. */
    template<>
    struct hash_64<u8>
    {
        [[nodiscard]] constexpr u64 operator()(const u8 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a i16 value. */
    template<>
    struct hash_64<i16>
    {
        [[nodiscard]] constexpr u64 operator()(const i16 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a u16 value. */
    template<>
    struct hash_64<u16>
    {
        [[nodiscard]] constexpr u64 operator()(const u16 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a i32 value. */
    template<>
    struct hash_64<i32>
    {
        [[nodiscard]] constexpr u64 operator()(const i32 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a u32 value. */
    template<>
    struct hash_64<u32>
    {
        [[nodiscard]] constexpr u64 operator()(const u32 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a i64 value. */
    template<>
    struct hash_64<i64>
    {
        [[nodiscard]] constexpr u64 operator()(const i64 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a u64 value. */
    template<>
    struct hash_64<u64>
    {
        [[nodiscard]] constexpr u64 operator()(const u64 value) const
        {
            return static_cast<u64>(value);
        }
    };

    /** Retrieves the 64 bits hash of a f32 value. */
    template<>
    struct hash_64<f32>
    {
        [[nodiscard]] constexpr u64 operator()(const f32 value) const
        {
            return static_cast<u64>(hud::bit_cast<u32>(value));
        }
    };

    /** Retrieves the 64 bits hash of a f64 value. */
    template<>
    struct hash_64<f64>
    {
        [[nodiscard]] constexpr u64 operator()(const f64 value) const
        {
            return hud::hash_64<u64> {}(hud::bit_cast<u64>(value));
        }
    };

    /** Retrieves the 64 bits hash of a ansichar null-terminated string. */
    template<>
    struct hash_64<const ansichar *, usize>
    {
        [[nodiscard]] constexpr u64 operator()(const ansichar *value, usize length) const
        {
            return hud::hash_algorithm::city_hash::hash_64(value, length);
        }
    };

    template<>
    struct hash_64<const ansichar *>
    {
        [[nodiscard]] constexpr u64 operator()(const ansichar *value, usize length) const
        {
            return hud::hash_64<const ansichar *, usize> {}(value, length);
        }
    };

    /** Retrieves the 64 bits hash of a wchar null-terminated string. */
    template<>
    struct hash_64<const wchar *, usize>
    {
        [[nodiscard]] inline u64 operator()(const wchar *value, usize length) const
        {
            return hud::hash_64<const ansichar *, usize> {}(reinterpret_cast<const ansichar *>(value), length * sizeof(wchar));
        }
    };

    template<>
    struct hash_64<const wchar *>
    {
        [[nodiscard]] inline u64 operator()(const wchar *value, usize length) const
        {
            return hud::hash_64<const wchar *, usize> {}(value, length);
        }
    };

    /** Retrieves the 64 bits hash of a pointer of a type type_t. */
    template<>
    struct hash_64<const void *>
    {
        [[nodiscard]] inline u64 operator()(const void *const pointer) const
        {
            const uptr ptr = reinterpret_cast<uptr>(pointer);
            if constexpr (sizeof(uptr) == 4)
            {
                return hud::hash_64<u32> {}(static_cast<u32>(ptr));
            }
            else if constexpr (sizeof(uptr) == 8)
            {
                return hud::hash_64<u64> {}(static_cast<u64>(ptr >> 4)); // 4 lowest bits in 64 bits are likely zero, ignore them
            }
        }
    };

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
            state_ = hud::combine_32(state_, hud::hash_32<hud::decay_t<type_t>...> {}(hud::forward<type_t>(values)...));
            return *this;
        }

        /** Retrieves the value of the `hasher_32`. */
        [[nodiscard]] constexpr operator u32() const noexcept
        {
            return state_;
        }

        /** Retrieves the value of the `hasher_32`. */
        [[nodiscard]] constexpr u32 result() const noexcept
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
            state_ = hud::combine_64(state_, hud::hash_64<hud::decay_t<type_t>...> {}(hud::forward<type_t>(values)...));
            return *this;
        }

        /** Retrieves the value of the `hasher_64`. */
        [[nodiscard]] constexpr operator u64() const noexcept
        {
            return state_;
        }

        /** Retrieves the value of the `hasher_64`. */
        [[nodiscard]] constexpr u64 result() const noexcept
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

    // Traits used to check if a type is hashable
    template<typename type_t, typename = void>
    struct is_hashable_64
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_hashable_64<type_t, void_t<decltype(hud::hash_64<hud::remove_cv_t<type_t>> {}(hud::declval<type_t>()))>>
        : hud::true_type
    {
    };

    /** Equivalent of is_hashable<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_hashable_64_v = is_hashable_64<type_t>::value;

    // Traits used to check if a type is hashable
    template<typename type_t, typename = void>
    struct is_hashable_32
        : hud::false_type
    {
    };

    template<typename type_t>
    struct is_hashable_32<type_t, void_t<decltype(hud::hash_32<hud::remove_cv_t<type_t>> {}(hud::declval<type_t>()))>>
        : hud::true_type
    {
    };

    /** Equivalent of is_hashable<type_t>::value. */
    template<typename type_t>
    inline constexpr bool is_hashable_32_v = is_hashable_32<type_t>::value;
} // namespace hud

#endif // HD_INC_CORE_HASH_H
