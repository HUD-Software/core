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
        using i128_impl = details::i128::i128_impl;

    public:
        using i128_impl::i128_impl;

        /** Construct a i128 from u128. */
        constexpr i128(u128 value) noexcept;

        constexpr i128(i128_impl impl) noexcept
            : i128_impl(impl)
        {
        }

        /** Retrieves the high part of the i128. */
        using i128_impl::high;
        /** Retrieves the low part of the i128. */
        using i128_impl::low;

        /** Cast to bool. */
        using i128_impl::operator bool;
        /** Cast to i8. */
        using i128_impl::operator i8;
        /** Cast to u8. */
        using i128_impl::operator u8;
        /** Cast to i16. */
        using i128_impl::operator i16;
        /** Cast to u16. */
        using i128_impl::operator u16;
        /** Cast to i32. */
        using i128_impl::operator i32;
        /** Cast to u32. */
        using i128_impl::operator u32;
        /** Cast to i64. */
        using i128_impl::operator i64;
        /** Cast to u64. */
        using i128_impl::operator u64;
        /** Cast to ansichar. */
        using i128_impl::operator ansichar;
        /** Cast to wchar. */
        using i128_impl::operator wchar;
        /** Cast to char16. */
        using i128_impl::operator char16;
        /** Cast to char32. */
        using i128_impl::operator char32;
        /** Cast to f32. */
        using i128_impl::operator f32;
        /** Cast to f64. */
        using i128_impl::operator f64;
#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        using i128_impl::operator __int128;
        /** Cast to unsigned __int128. */
        using i128_impl::operator unsigned __int128;
#endif
    };

    class alignas(16) u128
        : public details::i128::u128_impl
    {
    private:
        using u128_impl = details::i128::u128_impl;

    public:
        using u128_impl::u128_impl;

        constexpr u128(i128 value) noexcept
            : u128_impl(static_cast<u128_impl>(value))
        {
        }

        constexpr u128(u128_impl impl) noexcept
            : u128_impl(impl)
        {
        }

        /** Retrieves the high part of the i128. */
        using u128_impl::high;
        /** Retrieves the low part of the i128. */
        using u128_impl::low;

        /** Cast to bool. */
        using u128_impl::operator bool;
        /** Cast to i8. */
        using u128_impl::operator i8;
        /** Cast to u8. */
        using u128_impl::operator u8;
        /** Cast to i16. */
        using u128_impl::operator i16;
        /** Cast to u16. */
        using u128_impl::operator u16;
        /** Cast to i32. */
        using u128_impl::operator i32;
        /** Cast to u32. */
        using u128_impl::operator u32;
        /** Cast to i64. */
        using u128_impl::operator i64;
        /** Cast to u64. */
        using u128_impl::operator u64;
        /** Cast to ansichar. */
        using u128_impl::operator ansichar;
        /** Cast to wchar. */
        using u128_impl::operator wchar;
        /** Cast to char16. */
        using u128_impl::operator char16;
        /** Cast to char32. */
        using u128_impl::operator char32;
        /** Cast to f32. */
        using u128_impl::operator f32;
        /** Cast to f64. */
        using u128_impl::operator f64;
#if HD_INTRINSIC_INT128_SUPPORTED
        /** Cast to __int128. */
        using u128_impl::operator __int128;
        /** Cast to unsigned __int128. */
        using u128_impl::operator unsigned __int128;
#endif
    };

    /** Construct a i128 from u128. */
    constexpr i128::i128(u128 value) noexcept
        : i128_impl(static_cast<i128_impl>(value))
    {
    }

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
#if defined(HD_GLOBAL_NAMESPACE_TYPES)
using u128 = hud::u128; // 128 bits unsigned integer
using i128 = hud::i128; // 128 bits signed integer
#endif
#endif // HD_INC_CORE_I128_H