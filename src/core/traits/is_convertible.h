#ifndef HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#define HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#include "integral_constant.h"

#include "is_void.h"
#include "../templates/declval.h"
#if !__has_builtin(__is_convertible_to)
    #include "disjunction.h"
    #include "is_void.h"
    #include "is_function.h"
    #include "is_array.h"
#endif

namespace hud
{
    // #if __has_builtin(__is_convertible_to)
    //     /** Checks whether from_t is implicitly convertible to to_t. */
    //     template<typename from_t, typename to_t>
    //     struct is_convertible
    //         : hud::bool_constant<__is_convertible_to(from_t, to_t)>
    //     {
    //     };
    // #else
    namespace details
    {

        template<class T>
        auto test_returnable(int) -> decltype(void(static_cast<T (*)()>(nullptr)), hud::true_type {});
        template<class>
        auto test_returnable(...) -> hud::false_type;

        template<class From, class To>
        auto test_implicitly_convertible(int) -> decltype(void(hud::declval<void (&)(To)>()(hud::declval<From>())), hud::true_type {});
        template<class, class>
        auto test_implicitly_convertible(...) -> hud::false_type;

    } // namespace details

    template<class From, class To>
    struct is_convertible
        : std::bool_constant<(decltype(details::test_returnable<To>(0))::value && decltype(details::test_implicitly_convertible<From, To>(0))::value) || (hud::is_void<From>::value && hud::is_void<To>::value)>
    {
    };

    // #endif

    /** Equivalent of hud::is_convertible<from_t,to_t>::value. */
    template<typename from_t, typename to_t>
    inline constexpr bool is_convertible_v = hud::is_convertible<from_t, to_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H