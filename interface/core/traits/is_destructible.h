#ifndef HD_INC_CORE_TRAITS_IS_DESTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_DESTRUCTIBLE_H
#include "integral_constant.h"
#if !defined(HD_COMPILER_MSVC) && !defined(HD_COMPILER_CLANG_CL)
    #include "../templates/declval.h"
    #include "conditional.h"
    #include "disjunction.h"
    #include "is_void.h"
    #include "is_unbounded_array.h"
    #include "is_function.h"
    #include "is_reference.h"
    #include "is_scalar.h"
    #include "void_t.h"
    #include "remove_all_extents.h"
#endif

namespace hud
{

/**
 * Checks whether type_t is a destructible type.
 * Destructible types include scalar types and destructible classes.
 * A destructible class is a class whose destructor is not deleted and potentially accessible in derived classes,
 * and whose base class and all non-static data members are also destructible themselves.
 */
#if defined(HD_COMPILER_MSVC) || defined(HD_COMPILER_CLANG_CL)
    template<typename type_t>
    struct is_destructible
        : hud::bool_constant<__is_destructible(type_t)>
    {
    };
#else

    namespace details
    {
        template<typename type_t, typename = void>
        struct is_destructor_well_formed
            : hud::false_type
        {
        };

        template<typename type_t>
        struct is_destructor_well_formed<type_t, void_t<decltype(hud::declval<type_t &>().~type_t())>>
            : hud::true_type
        {
        };
    } // namespace details

    /**
     * Checks whether type_t is a destructible type.
     * Destructible types include scalar types and destructible classes.
     * A destructible class is a class whose destructor is not deleted and potentially accessible in derived classes,
     * and whose base class and all non-static data members are also destructible themselves.
     */
    template<typename type_t>
    struct is_destructible
        : hud::conditional_t<
              hud::disjunction_v<
                  is_void<type_t>,
                  is_unbounded_array<type_t>,
                  is_function<type_t>>,
              hud::false_type, // false if void, unbounded array, or function ignoring cv-qualifiers
              hud::conditional_t<
                  hud::disjunction_v<is_reference<type_t>, is_scalar<type_t>>,
                  hud::true_type,                                                       // true if reference or scalar
                  details::is_destructor_well_formed<hud::remove_all_extents_t<type_t>> // else check if calling the constructor is well-formed
                  >>
    {
    };

#endif

    /** Equivalent of is_destructible<type_t>::value. */
    template<typename type_t>
    constexpr bool is_destructible_v = is_destructible<type_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_DESTRUCTIBLE_H