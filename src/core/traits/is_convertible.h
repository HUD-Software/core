#ifndef HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#define HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H
#include "integral_constant.h"
#if !__has_builtin(__is_convertible_to)
    #include "disjunction.h"
    #include "is_void.h"
    #include "is_function.h"
    #include "is_array.h"
#endif

namespace hud
{
#if __has_builtin(__is_convertible_to)
    /** Checks whether from_t is implicitly convertible to to_t. */
    template<typename from_t, typename to_t>
    struct is_convertible
        : hud::bool_constant<__is_convertible_to(from_t, to_t)>
    {
    };
#else
    namespace details
    {
        template<typename from_t, typename to_t, bool = hud::disjunction<hud::is_void<from_t>, hud::is_function<to_t>, hud::is_array<to_t>>::value>
        struct is_convertible
        {
            typedef typename hud::is_void<to_t>::type type;
        };

        template<typename from_t, typename to_t>
        class is_convertible<from_t, to_t, false>
        {
            template<typename to_v_t>
            static void __test_aux(to_v_t) noexcept;

            template<typename from_v_t, typename to_v_t, typename = decltype(__test_aux<to_v_t>(std::declval<from_v_t>()))>
            static true_type
            __test(int);

            template<typename, typename>
            static false_type
            __test(...);

        public:
            typedef decltype(__test<from_t, to_t>(0)) type;
        };
    } // namespace details

    /** Checks whether from_t is implicitly convertible to to_t. */
    template<typename from_t, typename to_t>
    struct is_convertible
        : hud::details::is_convertible<from_t, to_t>::type
    {
    };
#endif

    /** Equivalent of hud::is_convertible<from_t,to_t>::value. */
    template<typename from_t, typename to_t>
    inline constexpr bool is_convertible_v = hud::is_convertible<from_t, to_t>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_CONVERTIBLE_H