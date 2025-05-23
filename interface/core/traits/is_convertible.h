#ifndef HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#define HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#include "integral_constant.h"

#if !HD_HAS_BUILTIN_IS_CONVERTIBLE_TO
    #include "disjunction.h"
    #include "is_void.h"
    #include "is_function.h"
    #include "is_array.h"
    #include "../templates/declval.h"

#endif

namespace hud
{
#if HD_HAS_BUILTIN_IS_CONVERTIBLE_TO
    /** Checks whether from_t is implicitly convertible to to_t. */
    template<typename from_t, typename to_t>
    struct is_convertible
        : hud::bool_constant<__is_convertible_to(from_t, to_t)>
    {
    };
#else
    namespace details
    {

        template<class type_t>
        auto test_returnable(int) -> decltype(void(static_cast<type_t (*)()>(nullptr)), hud::true_type {});
        template<class>
        auto test_returnable(...) -> hud::false_type;

        template<class from_t, class to_t>
        auto test_implicitly_convertible(int) -> decltype(void(hud::declval<void (&)(to_t)>()(hud::declval<from_t>())), hud::true_type {});
        template<class, class>
        auto test_implicitly_convertible(...) -> hud::false_type;

    } // namespace details

    template<class from_t, class to_t>
    struct is_convertible
        : hud::bool_constant<(decltype(details::test_returnable<to_t>(0))::value && decltype(details::test_implicitly_convertible<from_t, to_t>(0))::value) || (hud::is_void<from_t>::value && hud::is_void<to_t>::value)>
    {
    };

#endif

    /**
     * Checks whether from_t is implicitly convertible to to_t.
     * Equivalent of hud::is_convertible<from_t,to_t>::value.
     * */
    template<typename from_t, typename to_t>
    constexpr bool is_convertible_v = hud::is_convertible<from_t, to_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H