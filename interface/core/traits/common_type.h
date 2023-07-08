#ifndef HD_INC_CORE_TRAITS_COMMON_TYPE_H
#define HD_INC_CORE_TRAITS_COMMON_TYPE_H
#include "decay.h"
#include "../templates/declval.h"
#include "remove_reference.h"
#include "void_t.h"

namespace hud
{

    /** Retrieves the common type for the types in the types list to which all of them can be converted. */
    template<typename... type_t>
    struct common_type;

    namespace details
    {

        template<typename type1_t, typename type2_t>
        using cond_t = decltype(false ? hud::declval<type1_t>() : hud::declval<type2_t>());

        template<typename type1_t, typename type2_t, typename CRT0 = const hud::remove_reference_t<type1_t> &, typename CRT1 = const hud::remove_reference_t<type2_t> &, typename = void>
        struct common_2_types_impl2
        {
        };

        template<typename type1_t, typename type2_t, typename CRT0, typename CRT1>
        struct common_2_types_impl2<type1_t, type2_t, CRT0, CRT1, void_t<hud::decay_t<cond_t<CRT0, CRT1>>>>
        {
            // If hud::decay_t<cond_t<CRT0, CRT1>> is valid, the type is hud::decay_t<cond_t<CRT0, CRT1>>
            using type = hud::decay_t<cond_t<CRT0, CRT1>>;
        };

        template<typename type1_t, typename type2_t, typename = void>
        struct common_2_types_impl
            : common_2_types_impl2<type1_t, type2_t>
        {
        };

        template<typename type1_t, typename type2_t>
        struct common_2_types_impl<type1_t, type2_t, void_t<hud::decay_t<cond_t<type1_t, type2_t>>>>
        {
            // If hud::decay_t<cond_t<type1_t,type2_t>> is valid, the type is hud::decay_t<cond_t<type1_t, type2_t>>
            using type = hud::decay_t<cond_t<type1_t, type2_t>>;
        };

        template<typename type1_t, typename type2_t, typename DecayedT0 = hud::decay_t<type1_t>, typename DecayedT1 = hud::decay_t<type2_t>>
        struct common_2_types
            : common_type<DecayedT0, DecayedT1>
        {
            // If hud::decay_t<type1_t> or hud::decay_t<type2_t> produces different type, the type is the same as common_type<DecayedT0, DecayedT1>
        };

        template<typename type1_t, typename type2_t>
        struct common_2_types<type1_t, type2_t, type1_t, type2_t>
            : common_2_types_impl<type1_t, type2_t>
        { // type1_t is DecayedT0, type2_t is DecayedT1
          // If hud::decay_t<type1_t> or hud::decay_t<type2_t> produces same type, the type is the same as common_type<DecayedT0, DecayedT1>
        };

        template<typename type1_t, typename type2_t>
        struct common_2_types<type1_t, type2_t, void_t<cond_t<type1_t, type2_t>>>
        {
            using type = hud::decay_t<cond_t<type1_t, type2_t>>;
        };

        template<typename Void, typename type1_t, typename type2_t, typename... rest_t>
        struct common_3_types
        {
        };

        template<typename type1_t, typename type2_t, typename... rest_t>
        struct common_3_types<void_t<typename common_type<type1_t, type2_t>::type>, type1_t, type2_t, rest_t...>
            : common_type<typename common_type<type1_t, type2_t>::type, rest_t...>
        {
            // If common_type<type1_t,type2_t>::type exist, the type is common_type<typename common_type<type1_t, type2_t>::type, rest_t...>
        };
    } // namespace details

    /**
     * Determines the common type among all types type_t..., that is the type all type_t... can be implicitly converted to.
     * If such a type exists (as determined according to the rules below), the member type names that type. Otherwise, there is no member type.
     * - If sizeof...(type_t) is zero, there is no member type.
     * - If sizeof...(type_t) is one (i.e., type_t... contains only one type type1_t), the member type names the same type as common_type<type1_t, type1_t>::type if it exists; otherwise there is no member type.
     * - If sizeof...(type_t) is two (i.e., type_t... contains exactly two types type1_t and type2_t),
     *   - If applying decay to at least one of type2_t and T2 produces a different type, the member type names the same type as common_type_t<hud::decay_t<type1_t>, hud::decay_t<type2_t>>, if it exists; if not, there is no member type.
     *   - Otherwise, if there is a user specialization for common_type<type1_t, type2_t>, that specialization is used;
     *   - Otherwise, if hud::decay_t<decltype(false ? hud::declval<type1_t>() : hud::declval<type2_t>())> is a valid type, the member type denotes that type;
     *   - Otherwise, if Decayt<decltype(false ? hud::declval<CR0>() : hud::declval<CR1>())> is a valid type, where CR0 and CR1 are const hud::remove_reference_t<type1_t>& and const hud::remove_reference_t<type2_t>& respectively, the member type denotes that type;
     *   - Otherwise, there is no member type.
     * - If sizeof...(type_t) is greater than two (i.e., type_t... consists of the types type1_t, type2_t, R...), then if common_type_t<type1_t, type2_t> exists, the member type denotes common_type_t<typename common_type_t<type1_t, type2_t>, R...> if such a type exists. In all other cases, there is no member type.
     */
    template<>
    struct common_type<>
    {
        // sizeof...(type_t) == 0
    };

    template<typename type_t>
    struct common_type<type_t>
        : common_type<type_t, type_t>
    {
        // sizeof...(type_t) == 1
    };

    template<typename type1_t, typename type2_t>
    struct common_type<type1_t, type2_t>
        : details::common_2_types<hud::decay_t<type1_t>, hud::decay_t<type2_t>>
    {
        // sizeof...(type_t) == 2
    };

    template<typename type1_t, typename type2_t, typename... rest_t>
    struct common_type<type1_t, type2_t, rest_t...>
        : details::common_3_types<void, type1_t, type2_t, rest_t...>
    {
        // sizeof...(type_t) > 2
    };

    /** Equivalent of typename common_type<args_t...>::type. */
    template<typename... type_t>
    using common_type_t = typename common_type<type_t...>::type;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_COMMON_TYPE_H