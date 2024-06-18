#ifndef HD_INC_CORE_I128_H
#define HD_INC_CORE_I128_H
#include "traits/conditional.h"
#include "traits/integral_constant.h"

#if !defined(HD_INC_CORE_TYPES_H)
    #error i128.h must be included after or in types.h
#endif

#if HD_INTRINSIC_INT128_SUPPORTED
    #include "i128/i128_intrinsics.h"
#else
    #include "i128/i128_portable.h"
#endif

namespace hud
{
    class i128;
    class u128;

    class alignas(16) i128
        : protected details::i128::i128_impl
    {
    private:
        using super = details::i128::i128_impl;

    public:
        /** Default constructor. */
        constexpr i128() = default;

        /** Construct a i128 from low and high part. */
        constexpr i128(i64 high, u64 low) noexcept
            : super(low, high)
        {
        }

        /** Constructor i128 from i32. */
        constexpr i128(i32 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u32. */
        constexpr i128(u32 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from i64. */
        constexpr i128(i64 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u64. */
        constexpr i128(u64 value) noexcept
            : super(value)
        {
        }

        /** Constructor i128 from u128. */
        explicit constexpr i128(u128 value) noexcept;

        /** Construct a i128 from f32. */
        i128(f32 value) noexcept
            : super(value)
        {
        }

        /** Construct a i128 from f64. */
        i128(f64 value) noexcept
            : super(value)
        {
        }

#if HD_INTRINSIC_INT128_SUPPORTED
        /** Construct a i128 from __int128. */
        i128(__int128 value) noexcept
            : super(value)
        {
        }

        /** Construct a i128 from __int128. */
        i128(unsigned __int128 value) noexcept
            : super(value)
        {
        }
#endif

        /** Retrieves the low part of the i128. */
        [[nodiscard]] constexpr u64 low() const noexcept
        {
            return super::low();
        }

        /** Retrieves the high part of the i128. */
        [[nodiscard]] constexpr i64 high() const noexcept
        {
            return super::high();
        }

        /** Cast to bool. */
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i8. */
        [[nodiscard]] constexpr explicit operator i8() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u8. */
        [[nodiscard]] constexpr explicit operator u8() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i16. */
        [[nodiscard]] constexpr explicit operator i16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u16. */
        [[nodiscard]] constexpr explicit operator u16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i32. */
        [[nodiscard]] constexpr explicit operator i32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u32. */
        [[nodiscard]] constexpr explicit operator u32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i64. */
        [[nodiscard]] constexpr explicit operator i64() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u64. */
        [[nodiscard]] constexpr explicit operator u64() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to ansichar. */
        [[nodiscard]] constexpr explicit operator ansichar() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to wchar. */
        [[nodiscard]] constexpr explicit operator wchar() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to char16. */
        [[nodiscard]] constexpr explicit operator char16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to char32. */
        [[nodiscard]] constexpr explicit operator char32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to f32. */
        [[nodiscard]] constexpr explicit operator f32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to f64. */
        [[nodiscard]] constexpr explicit operator f64() const noexcept
        {
            return *static_cast<const super *>(this);
        }
#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        [[nodiscard]] constexpr explicit operator __int128() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to unsigned __int128. */
        [[nodiscard]] constexpr explicit operator unsigned __int128() const noexcept
        {
            return *static_cast<const super *>(this);
        }
#endif
    };

    /**
     * Checks whether right and left i128 are equal.
     * @param left The left i128 to compare
     * @param right The right i128 to compare
     * @param true if right and left i128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const i128 &left, const i128 &right) noexcept
    {
        return left.low() == right.low() && left.high() == right.high();
    }

    /**
     * Checks whether right and left i128 are not equal.
     * @param left The left i128 to compare
     * @param right The right i128 to compare
     * @param true if right and left i128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const i128 &left, const i128 &right) noexcept
    {
        return !(left == right);
    }

    static inline constexpr i128 i128_max = i128 {i64_max, u64_max};
    static inline constexpr i128 i128_min = i128 {i64_min, 0u};

    class alignas(16) u128
        : protected details::i128::u128_impl
    {
    private:
        using super = details::i128::u128_impl;

    public:
        /** Default constructor. */
        constexpr u128() = default;

        /** Construct a u128 from low and high part. */
        constexpr u128(u64 high, u64 low) noexcept
            : super(low, high)
        {
        }

        /** Constructor u128 from i32. */
        constexpr u128(i32 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u32. */
        constexpr u128(u32 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from i64. */
        constexpr u128(i64 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u64. */
        constexpr u128(u64 value) noexcept
            : super(value)
        {
        }

        /** Constructor u128 from u128. */
        explicit constexpr u128(i128 value) noexcept
            : super(value)
        {
        }

        /** Construct a u128 from f32. */
        u128(f32 value) noexcept
            : super(value)
        {
        }

        /** Construct a u128 from f64. */
        u128(f64 value) noexcept
            : super(value)
        {
        }

#if HD_INTRINSIC_INT128_SUPPORTED
        /** Construct a i128 from __int128. */
        u128(__int128 value) noexcept
            : super(value)
        {
        }

        /** Construct a i128 from __int128. */
        u128(unsigned __int128 value) noexcept
            : super(value)
        {
        }
#endif

        /** Retrieves the low part of the u128. */
        [[nodiscard]] constexpr u64 low() const noexcept
        {
            return super::low();
        }

        /** Retrieves the high part of the u128. */
        [[nodiscard]] constexpr u64 high() const noexcept
        {
            return super::high();
        }

        /** Cast to bool. */
        [[nodiscard]] constexpr explicit operator bool() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i8. */
        [[nodiscard]] constexpr explicit operator i8() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u8. */
        [[nodiscard]] constexpr explicit operator u8() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i16. */
        [[nodiscard]] constexpr explicit operator i16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u16. */
        [[nodiscard]] constexpr explicit operator u16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i32. */
        [[nodiscard]] constexpr explicit operator i32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u32. */
        [[nodiscard]] constexpr explicit operator u32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to i64. */
        [[nodiscard]] constexpr explicit operator i64() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to u64. */
        [[nodiscard]] constexpr explicit operator u64() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to ansichar. */
        [[nodiscard]] constexpr explicit operator ansichar() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to wchar. */
        [[nodiscard]] constexpr explicit operator wchar() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to char16. */
        [[nodiscard]] constexpr explicit operator char16() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to char32. */
        [[nodiscard]] constexpr explicit operator char32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to f32. */
        [[nodiscard]] constexpr explicit operator f32() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to f64. */
        [[nodiscard]] constexpr explicit operator f64() const noexcept
        {
            return *static_cast<const super *>(this);
        }

#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        [[nodiscard]] constexpr explicit operator __int128() const noexcept
        {
            return *static_cast<const super *>(this);
        }

        /** Cast to unsigned __int128. */
        [[nodiscard]] constexpr explicit operator unsigned __int128() const noexcept
        {
            return *static_cast<const super *>(this);
        }
#endif
    };

    /**
     * Checks whether right and left u128 are equal.
     * @param left The left u128 to compare
     * @param right The right u128 to compare
     * @param true if right and left u128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator==(const u128 &left, const u128 &right) noexcept
    {
        return left.low() == right.low() && left.high() == right.high();
    }

    /**
     * Checks whether right and left u128 are not equal.
     * @param left The left u128 to compare
     * @param right The right u128 to compare
     * @param true if right and left u128 are equal, false otherwise
     */
    [[nodiscard]] HD_FORCEINLINE constexpr bool operator!=(const u128 &left, const u128 &right) noexcept
    {
        return !(left == right);
    }

    static inline constexpr u128 u128_max = u128 {u64_max, u64_max};
    static inline constexpr u128 u128_min = u128 {u64_min, 0u};

#if HD_INTRINSIC_INT128_SUPPORTED
    namespace details::i128
    {
        /** Construct a i128 from i128. */
        constexpr i128_intrinsics::i128_intrinsics(hud::u128 value) noexcept
            : value_(static_cast<__int128>(value))
        {
        }

        constexpr u128_intrinsics::u128_intrinsics(hud::i128 value) noexcept
            : value_(static_cast<unsigned __int128>(value))
        {
        }

    } // namespace details::i128
#else
    namespace details::i128
    {
        /** Construct a i128 from u128. */
        constexpr i128_little::i128_little(hud::u128 value) noexcept
            : low_(value.low())
            , high_(static_cast<i64>(value.high()))
        {
        }

        /** Construct a i128 from u128. */
        constexpr i128_big::i128_big(hud::u128 value) noexcept
            : high_(static_cast<i64>(value.high()))
            , low_(value.low())
        {
        }

        constexpr u128_little::u128_little(hud::i128 value) noexcept
            : low_(value.low())
            , high_(static_cast<u64>(value.high()))
        {
        }

        constexpr u128_big::u128_big(hud::i128 value) noexcept
            : high_(static_cast<u64>(value.high()))
            , low_(value.low())
        {
        }
    } // namespace details::i128

#endif

    constexpr i128::i128(u128 value) noexcept
        : super(value)
    {
    }

} // namespace hud

#endif // HD_INC_CORE_I128_H