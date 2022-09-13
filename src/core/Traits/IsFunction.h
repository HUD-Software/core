#pragma once
#ifndef HD_INC_OSLAYER_TRAITS_IS_FUNCTION_H
#define HD_INC_OSLAYER_TRAITS_IS_FUNCTION_H
#include "IntegralConstant.h"

namespace hud {

    /** Checks whether T is a function type. Types like std::function, lambdas, classes with overloaded operator() and pointers to functions don't count as function types. */
    template <typename T, typename... TArgs>
    struct IsFunction
        : FalseType {
    };

    /** Specialization for regular functions */
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...)> : TrueType {};

    /** Specialization for variadic functions */
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...)> : TrueType {};

    /** Specialization for regular function that have cv-qualifiers */
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) volatile> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const volatile> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) volatile> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const volatile> : TrueType {};

    /** Specialization for function types that have ref-qualifiers */
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...)&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...)&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) volatile&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const volatile&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) volatile&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const volatile&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...)&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...)&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) volatile&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs...) const volatile&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) volatile&&> : TrueType {};
    template < typename T, typename... TArgs>
    struct IsFunction<T(TArgs..., ...) const volatile&&> : TrueType {};

    /** Equivalent of IsFunction<T, TArgs...>::Value. */
    template < typename T, typename... TArgs>
    inline constexpr bool IsFunctionV = IsFunction<T, TArgs...>::Value;

} // namespace hud

#endif // HD_INC_OSLAYER_TRAITS_IS_FUNCTION_H