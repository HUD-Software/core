#ifndef HD_INC_CORE_I128_H
#define HD_INC_CORE_I128_H
#include "traits/conditional.h"
#include "traits/integral_constant.h"
#include "traits/is_signed.h"
#include "traits/is_unsigned.h"

#if !defined(HD_INC_CORE_TYPES_H)
    #error i128.h must be included after or in types.h
#endif

#if HD_INTRINSIC_INT128_SUPPORTED
    #include "i128/i128_intrinsics.h"
#else
    #include "i128/i128_portable.h"
#endif
#include "limits.h"

namespace hud
{
    class i128;
    class u128;

    class alignas(16) i128
        : public details::i128::i128_impl
    {
    private:
        using super = details::i128::i128_impl;

    public:
        using super::super;

        /** Construct a i128 from u128. */
        constexpr i128(u128 value) noexcept;

        /** Retrieves the high part of the i128. */
        using super::high;
        /** Retrieves the low part of the i128. */
        using super::low;

        /** Cast to bool. */
        using super::operator bool;
        /** Cast to i8. */
        using super::operator i8;
        /** Cast to u8. */
        using super::operator u8;
        /** Cast to i16. */
        using super::operator i16;
        /** Cast to u16. */
        using super::operator u16;
        /** Cast to i32. */
        using super::operator i32;
        /** Cast to u32. */
        using super::operator u32;
        /** Cast to i64. */
        using super::operator i64;
        /** Cast to u64. */
        using super::operator u64;
        /** Cast to ansichar. */
        using super::operator ansichar;
        /** Cast to wchar. */
        using super::operator wchar;
        /** Cast to char16. */
        using super::operator char16;
        /** Cast to char32. */
        using super::operator char32;
        /** Cast to f32. */
        using super::operator f32;
        /** Cast to f64. */
        using super::operator f64;
#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        using super::operator __int128;
        /** Cast to unsigned __int128. */
        using super::operator unsigned __int128;
#endif
    };

    class alignas(16) u128
        : public details::i128::u128_impl
    {
    private:
        using super = details::i128::u128_impl;

    public:
        using super::super;

        constexpr u128(i128 value) noexcept
            : super(static_cast<super>(value))
        {
        }

        /** Retrieves the high part of the i128. */
        using super::high;
        /** Retrieves the low part of the i128. */
        using super::low;

        /** Cast to bool. */
        using super::operator bool;
        /** Cast to i8. */
        using super::operator i8;
        /** Cast to u8. */
        using super::operator u8;
        /** Cast to i16. */
        using super::operator i16;
        /** Cast to u16. */
        using super::operator u16;
        /** Cast to i32. */
        using super::operator i32;
        /** Cast to u32. */
        using super::operator u32;
        /** Cast to i64. */
        using super::operator i64;
        /** Cast to u64. */
        using super::operator u64;
        /** Cast to ansichar. */
        using super::operator ansichar;
        /** Cast to wchar. */
        using super::operator wchar;
        /** Cast to char16. */
        using super::operator char16;
        /** Cast to char32. */
        using super::operator char32;
        /** Cast to f32. */
        using super::operator f32;
        /** Cast to f64. */
        using super::operator f64;
#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        using super::operator __int128;
        /** Cast to unsigned __int128. */
        using super::operator unsigned __int128;
#endif
    };

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

#if HD_INTRINSIC_INT128_SUPPORTED

    template<> struct limits<__int128>
    {
        static constexpr __int128 min {static_cast<__int128>(i128_min)};
        static constexpr __int128 max {static_cast<__int128>(i128_max)};
    };

    template<> struct limits<unsigned __int128>
    {
        static constexpr unsigned __int128 min {static_cast<unsigned __int128>(u128_min)};
        static constexpr unsigned __int128 max {static_cast<unsigned __int128>(u128_max)};
    };

#endif

    template<>
    struct is_signed<i128>
        : hud::true_type
    {
    };

    template<>
    struct is_unsigned<u128>
        : hud::true_type
    {
    };
} // namespace hud

#endif // HD_INC_CORE_I128_H