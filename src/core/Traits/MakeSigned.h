#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_MAKE_SIGNED_H
#define HD_INC_OSLAYER_TRAITS_MAKE_SIGNED_H
#include "Or.h"
#include "And.h"
#include "IsIntegral.h"
#include "Not.h"
#include "IsBool.h"
#include "IsEnum.h"
#include "UnderlyingType.h"
#include "RemoveCV.h"
#include "IsConst.h"
#include "IsVolatile.h"
#include "ApplyCV.h"

namespace hud {

    namespace details {

        /** Retrieves the signed integral corresponding to U, keeping any cv-qualifiers. */
        template<typename U, usize>
        struct MakeIntegralSigned;

        template<typename U>
        struct MakeIntegralSigned<U, 1> {
            using Type = signed char;
        };

        template<typename U>
        struct MakeIntegralSigned<U, 2> {
            using Type = signed short;
        };

        template<typename U>
        struct MakeIntegralSigned<U, 4> {
            using Type = signed int;
        };

        template<>
        struct MakeIntegralSigned<long, 4> {
            using Type = signed long;
        };

        template<>
        struct MakeIntegralSigned<unsigned long, 4> {
            using Type = signed long;
        };

        template<typename U>
        struct MakeIntegralSigned<U, 8> {
            using Type = signed long long;
        };

        template<>
        struct MakeIntegralSigned<long, 8> {
            using Type = signed long;
        };

        template<>
        struct MakeIntegralSigned<unsigned long, 8> {
            using Type = signed long;
        };

        template<typename U>
        using MakeIntegralSignedT = typename MakeIntegralSigned<U, sizeof(U)>::Type;

        /** Retrieves the signed integral corresponding to T, keeping any cv-qualifiers if T is not an enum. */
        template<typename U, bool = IsEnumV<U>>
        struct MakeSignedImpl 
            : MakeSignedImpl<UnderlyingTypeT<U>> {
        };

        template<typename U>
        struct MakeSignedImpl<U, false> 
            : ApplyCV<MakeIntegralSignedT<RemoveCVT<U>>, IsConstV<U>, IsVolatileV<U>> {
        };

        template<typename U>
        using MakeSignedImplT = typename MakeSignedImpl<U>::Type;

    } // namespace details

    /**
    * Retrieves the signed type corresponding to T, keeping any cv-qualifiers.
    * If T is an integral (except bool) or enumeration type, provides the member typedef type which is the signed integer type corresponding to T, with the same cv-qualifiers.
    * If T is signed or unsigned char, short, int, long, long long; the signed type from this list corresponding to T is provided.
    * The T is an enumeration type or char, wchar_t, char8_t, char16_t, char32_t; the signed integer type with the smallest rank having the same sizeof as T is provided without cv-qualifiers.
    * Otherwise, the behavior is undefined.
    */
    template<typename T>
    struct MakeSigned
        : details::MakeSignedImpl<T> {
        static_assert(OrV< And<IsIntegral<T>, Not<IsBool<T>>>, IsEnum<T> >, "MakeSigned<T> requires that T shall be a (possibly cv-qualified) integral type or enumeration but not a bool type.");
    };

    /** Equivalent of typename MakeSigned<T>::Type. */
    template<typename T>
    using MakeSignedT = typename MakeSigned<T>::Type;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_MAKE_SIGNED_H
