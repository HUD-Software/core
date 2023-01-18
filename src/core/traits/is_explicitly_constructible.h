#ifndef HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#define HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H
#include "../traits/is_constructible.h"
#include "../traits/is_convertible.h"
#include "../traits/void_t.h"
#include "../templates/declval.h"

namespace hud
{

    namespace details
    {
        template<typename type_t>
        constexpr void implicit_construct(const type_t &) noexcept;

        template<typename void_t, bool is_constructible, typename type_t, typename... args_t>
        struct is_explicitly_constructible_impl_2
            : public hud::bool_constant<is_constructible>
        {
        };

        template<typename type_t, typename... args_t>
        struct is_explicitly_constructible_impl_2<hud::void_t<decltype(details::implicit_construct<type_t>({hud::declval<args_t>()...}))>, true, type_t, args_t...>
            : public hud::false_type
        {
        };

        template<typename type_t, typename... args_t>
        struct is_explicitly_constructible_impl
            : is_explicitly_constructible_impl_2<void, hud::is_constructible_v<type_t, args_t...>, type_t, args_t...>
        {
        };

        template<typename type_t>
        struct is_explicitly_constructible_impl<type_t, const type_t &>
            : hud::bool_constant<!hud::is_convertible_v<const type_t &, type_t>>
        {
        };

        template<typename type_t>
        struct is_explicitly_constructible_impl<type_t, type_t &&>
            : hud::bool_constant<!hud::is_convertible_v<type_t &&, type_t>>
        {
        };

    } // namespace details

    /** Checks whether type_t is an explicitly constructible type with args_t. */
    template<typename type_t, typename... args_t>
    struct is_explicitly_constructible
        : details::is_explicitly_constructible_impl<type_t, args_t...>
    {
    };

    /** Equivalent of is_explicitly_constructible<type_t, args_t...>::value. */
    template<typename type_t, typename... args_t>
    inline constexpr bool is_explicitly_constructible_v = is_explicitly_constructible<type_t, args_t...>::value;

} // namespace hud

#endif // HD_INC_CORE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_H