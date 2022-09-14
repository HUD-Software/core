#pragma once
#ifndef HD_INC_OSLAYER_traits_IS_DESTRUCTIBLE_H
#define HD_INC_OSLAYER_traits_IS_DESTRUCTIBLE_H
#include "integral_constant.h"
#if !defined(HD_COMPILER_MSVC) && !defined(HD_COMPILER_CLANG_CL)
#include "../templates/declval.h"
#include "conditional.h"
#include "or.h"
#include "is_void.h"
#include "is_unbounded_array.h"
#include "is_function.h"
#include "is_reference.h"
#include "is_scalar.h"
#include "void_t.h"
#include "remove_all_extents.h"
#endif

namespace hud {

    /**
    * Checks whether T is a destructible type.
    * Destructible types include scalar types and destructible classes.
    * A destructible class is a class whose destructor is not deleted and potentially accessible in derived classes,
    * and whose base class and all non-static data members are also destructible themselves.
    */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    template<typename T>
    struct IsDestructible
        : BoolConstant<__is_destructible(T)> {
    };
#else

    namespace details {
        template<typename T, typename = void>
        struct IsDestructorWellFormed
            : FalseType {
        };
        template<typename T>
        struct IsDestructorWellFormed<T, VoidT<decltype(hud::declval<T&>().~T())>>
            : TrueType {
        };
    }

    /**
    * Checks whether T is a destructible type.
    * Destructible types include scalar types and destructible classes.
    * A destructible class is a class whose destructor is not deleted and potentially accessible in derived classes,
    * and whose base class and all non-static data members are also destructible themselves.
    */
    template<typename T>
    struct IsDestructible
        : ConditionalT <
        OrV<
        IsVoid<T>,
        IsUnboundedArray<T>,
        IsFunction<T>
        >,
        FalseType, // false if void, unbounded array, or function ignoring cv-qualifiers
        ConditionalT<
        OrV<IsReference<T>, IsScalar<T>>,
        TrueType, // true if reference or scalar
        details::IsDestructorWellFormed<RemoveAllExtentsT<T>> // else check if calling the constructor is well-formed
        >> {
    };

#endif

    /** Equivalent of IsDestructible<T>::Value. */
    template<typename T>
    inline constexpr bool IsDestructibleV = IsDestructible<T>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_traits_IS_DESTRUCTIBLE_H