#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_MAKE_UNSIGNED_H
#define HD_INC_OSLAYER_TRAITS_MAKE_UNSIGNED_H
#include "Or.h"
#include "And.h"
#include "IsIntegral.h"
#include "Not.h"
#include "IsBool.h"
#include "IsEnum.h"
#include "Conditional.h"
#include "UnderlyingType.h"
#include "RemoveCV.h"
#include "IsConst.h"
#include "IsVolatile.h"
#include "ApplyCV.h"

namespace hud {

    namespace details {

        /**  Retrieves the unsigned integral corresponding to U, keeping any cv-qualifiers. */
        template<typename U, usize>
        struct MakeIntegralUnsigned;

        template<typename U>
        struct MakeIntegralUnsigned<U, 1> {
            using Type = unsigned char;
        };

        template<typename U>
        struct MakeIntegralUnsigned<U, 2> {
            using Type = unsigned short;
        };

        template<typename U>
        struct MakeIntegralUnsigned<U, 4> {
            using Type = unsigned int;
        };

        template<>
        struct MakeIntegralUnsigned<long, 4> {
            using Type = unsigned long;
        };

        template<>
        struct MakeIntegralUnsigned<unsigned long, 4> {
            using Type = unsigned long;
        };

        template<typename U>
        struct MakeIntegralUnsigned<U, 8> {
            using Type = unsigned long long;
        };

        template<>
        struct MakeIntegralUnsigned<long, 8> {
            using Type = unsigned long;
        };

        template<>
        struct MakeIntegralUnsigned<unsigned long, 8> {
            using Type = unsigned long;
        };

        template<typename U>
        using MakeIntegralUnsignedT = typename MakeIntegralUnsigned<U, sizeof(U)>::Type;


        /**  Retrieves the unsigned integral corresponding to U, keeping any cv-qualifiers if U is not an enum. */
        template<typename U, bool = IsEnumV<U>>
        struct MakeUnsignedImpl
            : MakeUnsignedImpl<UnderlyingTypeT<U>> {
        };

        template<typename U>
        struct MakeUnsignedImpl<U, false>
            : ApplyCV<MakeIntegralUnsignedT<RemoveCVT<U>>, IsConstV<U>, IsVolatileV<U>> {
        };

        template<typename U>
        using MakeUnsignedImplT = typename MakeUnsignedImpl<U>::Type;

    } // namespace details

    /**
    * Retrieves the unsigned type corresponding to T, keeping any cv-qualifiers.
    * If T is an integral (except bool) or enumeration type, provides the member typedef type which is the unsigned integer type corresponding to T, with the same cv-qualifiers.
    * If T is signed or unsigned char, short, int, long, long long; the unsigned type from this list corresponding to T is provided.
    * The T is an enumeration type or char, wchar_t, char8_t, char16_t, char32_t; the unsigned integer type with the smallest rank having the same sizeof as T is provided.
    * Otherwise, the behavior is undefined.
    */
    template<typename T>
    struct MakeUnsigned
        : details::MakeUnsignedImpl<T> {
        static_assert(OrV< And<IsIntegral<T>, Not<IsBool<T>>>, IsEnum<T> >, "MakeUnsigned<T> requires that T shall be a (possibly cv-qualified) integral type or enumeration but not a bool type.");
    };

    /** Equivalent of typename MakeUnsigned<T>::Type. */
    template<typename T>
    using MakeUnsignedT = typename MakeUnsigned<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_MAKE_UNSIGNED_H
