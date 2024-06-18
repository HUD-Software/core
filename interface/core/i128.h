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
#include "math.h"

namespace hud
{
    class i128;
    class u128;

    class alignas(16) i128
        : protected details::i128::i128_impl
    {

    private:
        friend u128;
        using super = details::i128::i128_impl;

    public:
        using super::super;

        /** Construct a i128 from u128. */
        explicit constexpr i128(u128 value) noexcept;

        /** Retrieves the high part of the i128. */
        using super::high;
        /** Retrieves the low part of the i128. */
        using super::low;

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

    class alignas(16) u128
        : protected details::i128::u128_impl
    {
    private:
        friend i128;
        using super = details::i128::u128_impl;

    public:
        using super::super;

        explicit constexpr u128(i128 value) noexcept
            : super(static_cast<super>(value))
        {
        }

        /** Retrieves the high part of the i128. */
        using super::high;
        /** Retrieves the low part of the i128. */
        using super::low;

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

    /** Construct a i128 from u128. */
    constexpr i128::i128(u128 value) noexcept
        : super(static_cast<super>(value))
    {
    }

#if !HD_INTRINSIC_INT128_SUPPORTED
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

    static inline constexpr i128 i128_max = i128 {i64_max, u64_max};
    static inline constexpr i128 i128_min = i128 {i64_min, 0u};

    static inline constexpr u128 u128_max = u128 {u64_max, u64_max};
    static inline constexpr u128 u128_min = u128 {u64_min, 0u};

    namespace math
    {
        template<> struct limits<i128>
        {
            static constexpr i128 min {hud::i128_min};
            static constexpr i128 max {hud::i128_max};
        };

        template<> struct limits<u128>
        {
            static constexpr u128 min {hud::u128_min};
            static constexpr u128 max {hud::u128_max};
        };
    } // namespace math

} // namespace hud

#endif // HD_INC_CORE_I128_H