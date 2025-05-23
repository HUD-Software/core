#ifndef HD_INC_CORE_TRAITS_IS_FUNCTION_H
#define HD_INC_CORE_TRAITS_IS_FUNCTION_H
#include "integral_constant.h"

namespace hud
{

    /** Checks whether type_t is a function type. types like std::function, lambdas, classes with overloaded operator() and pointers to functions don't count as function types. */
    template<typename type_t, typename... args_t>
    struct is_function
        : hud::false_type
    {
    };

    /** Specialization for regular functions */
    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...)> : hud::true_type
    {
    };

    /** Specialization for variadic functions */
    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...)> : hud::true_type
    {
    };

    /** Specialization for regular function that have cv-qualifiers */
    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) volatile> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const volatile> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) volatile> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const volatile> : hud::true_type
    {
    };

    /** Specialization for function types that have ref-qualifiers */
    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) volatile &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const volatile &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) volatile &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const volatile &> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) volatile &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t...) const volatile &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) volatile &&> : hud::true_type
    {
    };

    template<typename type_t, typename... args_t>
    struct is_function<type_t(args_t..., ...) const volatile &&> : hud::true_type
    {
    };

    /** Equivalent of is_function<type_t, args_t...>::value. */
    template<typename type_t, typename... args_t>
    constexpr bool is_function_v = is_function<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_FUNCTION_H